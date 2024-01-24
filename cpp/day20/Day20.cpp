#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <variant>

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
		std::vector<Signal> process(signal signal) {
			if (signal.strength == Signal::Strength.high) return std::vector<Signal>();

			std::vector<Signal> result;			
			isOn = !isOn;
			if (isOn) {
				//send high pulse
				for (auto destination : destinations) {
					result.push_back(Signal{ Signal::Strength.high, label, destination });
				}
			}
			else {
				//send low pulse
				for (auto destination : destinations) {
					result.push_back(Signal{ Signal::Strength.low, label, destination });
				}
			}
			return result;
		}
	};

	struct Conjunction {
		std::vector<std::string> destinations;
		std::map<std::string, bool> inputs;
		//remember the most recent pulse received from each connected input module
		//initially default to a low pulse for each input
		bool mostRecentPulseTypeIsHigh = false;

		void AddInput(std::string in) {
			inputs.push_back(in, false);
		}

		//when receiving, first update memory on most recent pulse type for that module
		//then if all inputs are high, send a low pulse
		//else send a high pulse

		bool isInitialState() {
			for ((std::string mod, bool pulse) : inputs) {
				if (pulse == true) return false;
			}
			return true;
		}
	};

	struct Broadcaster {
		const std::string label = "broadcaster";		
		std::vector<std::string> destinations;
		//when it recieves a pulse, it sends the same pulse to all destination modules
	};

	struct Button {
		const std::string label = "button";
		const std::string broadcasterLabel = "broadcaster";
		//when pushed, send a single low pulse to the broadcaster module
		std::vector<Signal> process() {
			return std::vector<Signal>{Signal{ Signal::Strength.low, label, broadcasterLabel }};
		}
	};
	

	struct modulesSystem {
		std::map<std::string, std::variant<FlipFlop, Conjunction, Broadcaster>> modules;
	};
}
