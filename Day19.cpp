// list of rules
//rule has a condition and destination if the condition is satisfied
//first rule that matches the part is applied and the part moves to the destination
// last rule has no condition and always applies if reached

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>

namespace {
	struct Part {
		int64_t x, m, a, s;		
		int64_t GetTotalRating() {
			return x + m + a + s;
		}
	};
	struct Rule {
		char category = ' ';
		char operative = ' ';
		int64_t rating = 0;
		std::string destination;

		Rule(std::string destination) : destination(destination){}
		Rule(char category, char op, int64_t rating, std::string destination): category(category), operative(op), rating(rating), destination(destination) {}

		int64_t getPartRating(Part part) {
			if (category == ' ') return 0;
			int64_t result;
			switch (category) {
			case 'x':
				result =  part.x;
				break;
			case 'm':
				result = part.m;
				break;
			case 'a':
				result = part.a;
				break;
			case 's':
				result = part.s;
				break;
			}
			return result;
		}

		bool checkCondition(Part part) {
			if (category == ' ') {
				return true; // our rule has no condition, presumably because it is the last rule in a workflow
			}
			
			bool result;
			int64_t actualRating = getPartRating(part);

			switch (operative) {
			case '>':
				result = actualRating > rating;
				break;
			case '<':
				result = actualRating < rating;
				break;
			}
			return result;
		}
	};
	struct Workflow {
		std::vector<Rule> rules;		
	};	
}

int main() {
	std::ifstream ifs("input.txt");
	std::string input;
	std::map<std::string, Workflow> workflows;
	std::vector<Part> parts;

	auto get_name = [](std::string in) {
		auto nameEndIndex = in.find("{");
		auto name = in.substr(0, nameEndIndex);
		return name;
	};

	auto make_workflow = [](std::string in) {

		std::vector<Rule> rules;

		std::regex rulePattern{ R"(([xmas])([\<\>])(\d+)\:(\w+)\,)" };
		std::sregex_iterator iter(in.begin(), in.end(), rulePattern);
		std::sregex_iterator end;

		while (iter != end) {
			auto matches = *iter;
			auto category = matches[1].str().at(0);
			auto op = matches[2].str().at(0);
			auto rating = std::stoi(matches[3].str());
			auto destination = matches[4].str();			
			rules.push_back(Rule{ category, op, rating, destination });
			++iter;
		}

		//get last rule
		std::regex lastRulePattern{ R"((\w+)\})" };
		std::smatch smatch;
		if (std::regex_search(in, smatch, lastRulePattern))
		{
			Rule rule{ smatch[1].str() };
			rules.push_back(rule);
		}

		return Workflow{rules};
	};

	auto make_part = [](std::string in) {
		std::vector<int64_t> ratings;
		std::regex partPattern{ R"([xmas]\=(\d+))" };
		std::sregex_token_iterator iter(in.begin(), in.end(), partPattern, 1);
		std::sregex_token_iterator end;

		while (iter != end) {			
			ratings.push_back(std::stoi(*iter));
			++iter;
		}
		return Part{ ratings[0], ratings[1], ratings[2], ratings[3] };
	};

	bool processWorkflowsInput = true;

	while (ifs.good()) {
		std::getline(ifs, input);
		if (input.empty())
		{
			processWorkflowsInput = false;
			continue;
		}
		if(processWorkflowsInput)
			workflows[get_name(input)] = make_workflow(input);
		else {
			parts.push_back(make_part(input));
		}
	}
	ifs.clear();

	auto is_part_accepted = [&](Part part) {
		std::string destination = "in";
		while (destination != "A") {
			if (destination == "R") return false;
			auto workflow = workflows[destination];
			for (auto rule : workflow.rules) {
				if (rule.checkCondition(part)) {
					destination = rule.destination;
					break;
				}		
			}
		}
		return true;
	};

	int64_t part1_sum = 0;
	for (auto part : parts) {
		if (is_part_accepted(part)) {
			part1_sum += part.GetTotalRating();
		}
	}
	std::cout << "part 1: " << part1_sum << '\n';
	
	return 0;
}