#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <variant>
#include <queue>
#include <regex>
#include <numeric>

namespace lcmHelper {
	template <typename M, typename N>
	constexpr auto lcm(const M& m, const N& n) {
		return std::lcm(m, n);
	}

	template <typename M, typename ...Rest>
	constexpr auto lcm(const M& first, const Rest&... rest) {
		return std::lcm(first, lcm(rest...));
	}
}

namespace {
	std::map<std::string, int64_t> inputMap;
	int64_t iterations = 0;

	struct Signal {
		enum Strength {low, high} strength;
		std::string from, to;
	};
	struct FlipFlop {
		std::string label;		
		std::vector<std::string> destinations;
		//initially off
		bool isOn = false;

		//if receive a high pulse, nothing happens
		//if low pulse, flip the isOn state
		//if was off, turn on and send high pulse
		//if was on, turn off and send low pulse
		std::vector<Signal> process(Signal signal) {
			if (signal.strength == Signal::high) return std::vector<Signal>();

			std::vector<Signal> result;			
			isOn = !isOn;
			if (isOn) {
				//send high pulse
				for (auto destination : destinations) {
					result.push_back(Signal{ Signal::high, label, destination });
				}
			}
			else {
				//send low pulse
				for (auto destination : destinations) {
					result.push_back(Signal{ Signal::low, label, destination });
				}
			}
			//std::cout << "process flipflop, destinations: " << '\n';
			return result;
		}
	};

	struct Conjunction {
		std::string label;
		std::vector<std::string> destinations;
		std::map<std::string, Signal::Strength> inputs;
		bool shouldRecord = false;
		
		//remember the most recent pulse received from each connected input module
		//initially default to a low pulse for each input		

		void AddInput(std::string in) {
			inputs[in] = Signal::low;
		}

		//when receiving, first update memory on most recent pulse type for that module
		//then if all inputs are high, send a low pulse
		//else send a high pulse
		std::vector<Signal> process(Signal signal) {
			auto inputModuleLabel = signal.from;
			inputs[inputModuleLabel] = signal.strength;

			std::vector<Signal> output;
			Signal::Strength signalStrength = Signal::low;
			for (auto [moduleLabel, pulseType] : inputs) {
				if (pulseType == Signal::low) {
					signalStrength = Signal::high;
					if (shouldRecord) inputMap[label] = iterations;
					break;
				}
			}

			for (auto destination : destinations) {
				output.push_back({ signalStrength, label, destination });
			}

			//std::cout << "process conjunction, destinations: " << '\n';
			return output;
		}

		//since all inputs are initalized to a low pulse, we are in the initial state if none of the signals are high
		bool isInitialState() const {
			for (auto [moduleLabel, pulseType] : inputs) {
				if (pulseType == Signal::high) return false;
			}
			return true;
		}
	};

	struct Broadcaster {
		std::string label = "broadcaster";
		std::vector<std::string> destinations;
		//when it recieves a pulse, it sends the same pulse to all destination modules

		std::vector<Signal> process(Signal signal) {			
			std::vector<Signal> output;
			for (auto destination : destinations) {
				output.push_back(Signal{ signal.strength, label, destination });
			}
			//std::cout << "process broadcaster, destinations: " << '\n';

			return output;
		}
	};

	struct Button {
		std::string label = "button";
		std::string broadcasterLabel = "broadcaster";
		//when pushed, send a single low pulse to the broadcaster module	
		std::vector<Signal> process() {
			//std::cout << "process button" << '\n';
			return std::vector<Signal>{Signal{ Signal::low, label, broadcasterLabel }};
		}
	};

	struct ProcessVisitor {
		Signal signal = Signal{ Signal::low, "button", "broadcaster" };
		std::vector<Signal> operator()(FlipFlop& flipFlop) {
			return flipFlop.process(signal);
		}
		std::vector<Signal> operator()(Broadcaster& broadcaster) {
			return broadcaster.process(signal);
		}
		std::vector<Signal> operator()(Conjunction& conjunction) {
			return conjunction.process(signal);
		}
		std::vector<Signal> operator()(Button& button) {
			return button.process();
		}
	};

	struct ContainsDestinationVistor
	{
		std::string destination = "";
		bool operator()(Broadcaster broadcaster) {
			for (auto dest : broadcaster.destinations) {
				if (dest == destination)
					return true;
			}
			return false;
		}
		bool operator()(FlipFlop flipFlop) {
			for (auto dest : flipFlop.destinations) {
				if (dest == destination)
					return true;
			}
			return false;
		}
		bool operator()(Conjunction conjunction) {
			for (auto dest : conjunction.destinations) {
				if (dest == destination)
					return true;
			}
			return false;
		}
		bool operator()(Button button) {
			return false;
		}
	};

	struct ModulesSystem {
		std::map<std::string, std::variant<FlipFlop, Conjunction, Broadcaster, Button>> modules;
		std::queue<Signal> pending;
		int64_t low_signals = 0, high_signals = 0;

		void startProcess() { 
			auto buttonPair = modules.find("button");			
			auto signals = std::visit(ProcessVisitor{}, buttonPair->second);
			for (auto signal : signals) {
				pending.push(signal);
				if (signal.strength == Signal::low) {
					++low_signals;
				}
				else {
					++high_signals;
				}
			}
			while (pending.size() > 0) {
				Signal signal = pending.front();
				//std::cout << pending.front().from << " -" << pending.front().strength << "-> " << pending.front().to << '\n';
				ProcessVisitor v{ signal };
				auto destinationPair = modules.find(signal.to);
				std::vector<Signal> signals;
				if (destinationPair != modules.end()) {
					signals = std::visit(v, destinationPair->second);
				}
				
				for (auto signal : signals) {
					pending.push(signal);
					if (signal.strength == Signal::low) {
						++low_signals;
					}
					else {
						++high_signals;
					}
				}
				pending.pop();
			}
		}
	};
}

int main() {
	std::vector<std::string> flipFlopLabels;
	std::vector<std::string> conjunctionLabels;

	ModulesSystem modSystem;
	Button b{};
	modSystem.modules[b.label] = b;

	std::ifstream ifs("input.txt");
	std::string input;
	std::map<std::string, Conjunction> conjunctions;
	std::regex destinationsPattern{ R"(([\w]+))" };

	while (ifs.good()) {
		//get module type and module label from left side of string
		std::getline(ifs, input);
		auto rightIndex = input.find("-> ");
		auto rightString = input.substr(rightIndex + 2);

		std::sregex_token_iterator iter(rightString.begin(), rightString.end(), destinationsPattern);
		std::sregex_token_iterator end;
		std::vector<std::string> destinations;
		while (iter != end) {
			destinations.push_back(*iter);
			++iter;
		}

		switch (input[0]) {
		case 'b':
		{
			Broadcaster broadcaster{};
			broadcaster.destinations = destinations;
			modSystem.modules[broadcaster.label] = broadcaster;
			break;
		}
		case '%':
		{
			FlipFlop flipFlop{};
			flipFlop.label = input.substr(1, rightIndex - 2);
			flipFlop.destinations = destinations;
			modSystem.modules[flipFlop.label] = flipFlop;
			break;
		}
		case '&':
		{
			Conjunction conjunction{};
			conjunction.label = input.substr(1, rightIndex - 2);
			conjunction.destinations = destinations;
			modSystem.modules[conjunction.label] = conjunction;
			conjunctions[conjunction.label] = conjunction;
			break;
		}
		}
	}
	ifs.clear();

	for (auto const [label, module] : modSystem.modules) {
		
		for (auto &conjunctionPair : conjunctions) {
			ContainsDestinationVistor cdV {conjunctionPair.first};
			bool containsConjunction = std::visit(cdV, module);
			if (containsConjunction) {
				conjunctionPair.second.AddInput(label);							
			}
		}		
	}

	for (auto &[label, conjunction] : conjunctions) {
		for (auto destination : conjunction.destinations) {
			if (destination == "qb") {
				inputMap[label] = 0;
				conjunction.shouldRecord = true;
			}
		}
		std::cout << " conjunction input size: " << conjunction.inputs.size() << '\n';
		modSystem.modules[label] = conjunction;
	}
	std::cout << "number of modules" << modSystem.modules.size() << '\n';

	int presses = 0;
	/*while (presses < 1000) {
		modSystem.startProcess();
		++presses;
		std::cout << presses << '\n';
	}

	std::cout << "low signals: " << modSystem.low_signals << ", high signals: " << modSystem.high_signals << ", product: " << modSystem.low_signals * modSystem.high_signals << '\n';*/

	//part 2

	while (presses < 5000) {
		++iterations;
		modSystem.startProcess();
		++presses;		
	}

	std::vector<int64_t> multiples;

	for (auto [label,highPeriod] : inputMap) {
		std::cout << label << ": " << highPeriod << '\n';
		multiples.push_back(highPeriod);
	}

	int64_t lcm = std::accumulate(multiples.begin(), multiples.end(), int64_t(1), [](int64_t a, int64_t b) {
		return std::lcm(a, b);
	});
	
	std::cout << "lcm: " << lcm << std::endl;

	return 0;
}
