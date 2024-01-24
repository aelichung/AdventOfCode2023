#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <variant>
#include <queue>
#include <regex>

namespace {
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
			return result;
		}
	};

	struct Conjunction {
		std::string label;
		std::vector<std::string> destinations;
		std::map<std::string, Signal::Strength> inputs;
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
					break;
				}
			}

			for (auto destination : destinations) {
				output.push_back({ signalStrength, label, destination });
			}
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
			return output;
		}
	};

	struct Button {
		std::string label = "button";
		std::string broadcasterLabel = "broadcaster";
		//when pushed, send a single low pulse to the broadcaster module	
		std::vector<Signal> process() {
			return std::vector<Signal>{Signal{ Signal::low, label, broadcasterLabel }};
		}
	};

	struct Visitor {
		Signal signal = Signal{ Signal::low, "button", "broadcaster" };
		std::vector<Signal> operator()(FlipFlop flipFlop) {
			return flipFlop.process(signal);
		}
		std::vector<Signal> operator()(Broadcaster broadcaster) {
			return broadcaster.process(signal);
		}
		std::vector<Signal> operator()(Conjunction conjunction) {
			return conjunction.process(signal);
		}
		std::vector<Signal> operator()(Button button) {
			return button.process();
		}
	};
	

	struct ModulesSystem {
		std::map<std::string, std::variant<FlipFlop, Conjunction, Broadcaster, Button>> modules;
		std::queue<Signal> pending;
		int64_t low_signals = 0, high_signals = 0;

		void startProcess() {
			auto buttonPair = modules.find("button");			
			auto signals = std::visit(Visitor{}, buttonPair->second);
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
				Visitor v{ signal };
				auto destinationPair = modules.find(signal.to);
				auto signals = std::visit(v, destinationPair->second);
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

	std::ifstream ifs ("input-test.txt");
	std::string input;
	std::regex moduleLabelPattern{ R"(([&%]*)([\w]+) )" };	
	std::regex destinationsPattern{ R"(([\w]+))" };

	while (ifs.good()) {
		//get module type and module label from left side of string
		std::getline(ifs, input);
		auto rightIndex = input.find("-> ");		
		auto rightString = input.substr(rightIndex + 2);
		std::smatch smatch;
		Broadcaster broadcaster{};
		FlipFlop flipFlop{};
		Conjunction conjunction{};

		std::sregex_token_iterator iter(rightString.begin(), rightString.end(), destinationsPattern);
		std::sregex_token_iterator end;
		switch (input[0]) {
		case 'b':
			while (iter != end) {				
				broadcaster.destinations.push_back(*iter);
				++iter;
			}
			modSystem.modules[broadcaster.label] = broadcaster;
			break;
		case '%':		
			flipFlop.label = input.substr(1, rightIndex -2);
			while (iter != end) {
				flipFlop.destinations.push_back(*iter);
				++iter;
			}
			modSystem.modules[flipFlop.label] = flipFlop;
			break;
		case '&':			
			conjunction.label = input.substr(1, rightIndex -2);
			while (iter != end) {
				conjunction.destinations.push_back(*iter);
				++iter;
			}
			modSystem.modules[conjunction.label] = conjunction;
			break;
		}		
	}

	for (auto labelPair : modSystem.modules) {
		std::cout << labelPair.first << '\n';
	}
	std::cout << modSystem.modules.size();
	
	return 0;
}
