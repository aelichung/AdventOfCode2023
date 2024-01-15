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
	struct Range {
		int64_t min, max;
	};
	struct PartRanges {
		Range x = {1,4000}, m = { 1,4000 }, a = { 1,4000 }, s = { 1,4000 };
		PartRanges EditRange(char c, Range range) {
			PartRanges result{ x,m,a,s };
			//more restrictive range stays
			switch (c) {
			case 'x':
				result.x = { std::max(range.min, x.min), std::min(range.max, x.max) };
				break;
			case 'm':				
				result.m = { std::max(range.min, m.min), std::min(range.max, m.max) };
				break;
			case 'a':								
				result.a = { std::max(range.min, a.min) , std::min(range.max, a.max) };
				break;
			case 's':								
				result.s = { std::max(range.min, s.min), std::min(range.max, s.max) };
				break;
			}
			return result;
		}

		int64_t GetPossibleCombinations() {
			int64_t product = 1;
			auto xRange = x.max - x.min + 1;
			auto mRange = m.max - m.min + 1;
			auto aRange = a.max - a.min + 1;
			auto sRange = s.max - s.min + 1;
			product = xRange * mRange * aRange * sRange;
			return product;
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
			
			bool result = true;
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

		Range acceptableRange() const{
			Range result{1,4000};
			switch (operative) {
			case '>':
				result = {rating + 1, 4000};
				break;
			case '<':
				result = { 1, rating - 1 };
				break;
			}
			return result;
		}

		//should only be called for rules preceding the last rule
		Range rejectedRange() const {
			if (category == ' ') std::cout << "no operation set, are we getting the rejected range for the last rule?"<<'\n';
			Range result{ 0,0 };
			switch (operative) {
			case '>':
				result = { 1, rating };
				break;
			case '<':
				result = { rating, 4000};
				break;
			}
			return result;
		}
	};
	struct Workflow {
		std::vector<Rule> rules;
	};	
}

//part 2
int64_t sum_combinations(std::map<std::string, Workflow>& workflows, std::string name, PartRanges partRanges) {	
	std::string destination = name;
	if (destination == "A") return partRanges.GetPossibleCombinations();
	if (destination == "R") return (int64_t)0;
	int64_t total = 0;
	auto workflow = workflows[destination];
	for (const auto& rule : workflow.rules) {
		//sucessful process:
		auto goodRange = rule.acceptableRange();		
		if (goodRange.min <= goodRange.max) {
			auto newRanges = partRanges.EditRange(rule.category, goodRange);
			total += sum_combinations(workflows, rule.destination, newRanges);
		}
		//failure process, only use on rules that have conditions (not the last rule)
		if (rule.category != ' ') {
			auto failRange = rule.rejectedRange();
			if (failRange.min <= failRange.max) {
				auto newRanges = partRanges.EditRange(rule.category, failRange);
				partRanges = newRanges;
			}
		}		
	}
	return total;
};

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

	PartRanges partRanges;
	auto sum = sum_combinations(workflows, "in", partRanges);

	std::cout << "part 2:" << sum << '\n';
	
	return 0;
}