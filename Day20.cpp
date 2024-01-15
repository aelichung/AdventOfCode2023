#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

namespace {
	struct flipFlop {
		std::vector<module> inputs;
		std::vector<module> destinations;
		//initially off
		bool isOn = false;

		//if receive a high pulse, nothing happens
		//if low pulse, flip the isOn state
		//if was off, turn on and send high pulse
		//if was on, turn off and send low pulse
		
	};

	struct conjunction {
		std::vector<module> destinations;
		std::map<module, bool> inputs;
		//remember the most recent pulse received from each connected input module
		//initially default to a low pulse for each input
		bool mostRecentPulseTypeIsHigh = false;

		void AddInput(module in) {
			inputs.push_back(in, false);
		}

		//when receiving, first update memory on most recent pulse type for that module
		//then if all inputs are high, send a low pulse
		//else send a high pulse

		bool isInitialState() {
			for ((module mod, bool pulse) : inputs) {
				if (pulse == true) return false;
			}
			return true;
		}
	};

	struct broadcast {
		std::vector<module> inputs;
		std::vector<module> destinations;
		//when it recieves a pulse, it sends the same pulse to all destination modules
	};

	struct button {
		module broadcaster;
		//when pushed, send a single low pulse to the broadcaster module
	};
	

	enum moduleType {
		flipFlop,
		conjunction, 
		broadcast,
		button,
		default
	};

	struct module {
		moduleType type;

	};
}
