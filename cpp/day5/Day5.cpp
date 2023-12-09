#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <vector>
#include <regex>

struct Range {
	long long start, end;	

	bool IsIntersection(Range range) {		
		//range 1 start is larger than range 2 start and
		//range 1 end is smaller range 2 end
		//return true

		if (start >= range.start && end <= range.end) {
			return true;
		}

		if (range.start >= start && range.end <= end) {
			return true;
		}
		

		//if range 1 start is less than range 2 start and 
		//range 1 end is greater than or equal to range 2 start and less than range 2 end
		//then return a new range with range 2 start and until range 1 end
		//return true

		if (start <= range.start && end >= range.start && end <= range.end) {
			return true;
		}

		if (range.start <= start && range.end >= start && range.end <= end) {
			return true;
		}

		//if none of these are satisfied, then there is no intersection
		return false;
	}

	Range Intersect(Range range) {			
		//range 1 start is larger than range 2 start and
		//range 1 end is smaller range 2 end
		//return range 1

		//range 1 and range 2 have the same values, return range 1

		if (start >= range.start && end <= range.end) {
			return Range(start, end);
		}

		if (range.start >= start && range.end <= end) {
			return range;
		}

		//if range 1 start is less than range 2 start and 
		//range 1 end is greater than or equal to range 2 start and less than range 2 end
		//then return a new range with range 2 start and until range 1 end

		if (start <= range.start && end >= range.start && end <= range.end) {
			return Range(range.start, end);
		}

		if (range.start <= start && range.end >= start && range.end <= end) {
			return Range(start, range.end);
		}

		//if none of these are satisfied, then there is no intersection
		return Range(-1, -1); //hopefully we don't run into this with the bool check!
	}

	std::vector<Range> Difference(Range inputRange) {
		std::vector<Range> diffRanges;

		if (start >= inputRange.start && end <= inputRange.end) {
			Range lesserRange = Range(inputRange.start, start-1);
			Range greaterRange = Range(end +1, inputRange.end);
			diffRanges.push_back(lesserRange);
			diffRanges.push_back(greaterRange);
			return diffRanges;
		}

		if (inputRange.start >= start && inputRange.end <= end) {
			//there is no meaningful difference in this case
			//do nothing
			return diffRanges;
		}

		if (start <= inputRange.start && end >= inputRange.start && end <= inputRange.end) {
			//the input inputRange is more to the right of this inputRange,
			//the meaningful difference is from the end of this inputRange to the end of the given inputRange
			Range diff = Range(end + 1, inputRange.end);
			diffRanges.push_back(diff);
			return diffRanges;
		}

		if (inputRange.start <= start && inputRange.end >= start && inputRange.end <= end) {
			//meaningful diff here is from the start of the given inputRange to the start of this inputRange
			Range diff = Range(inputRange.start, start-1);
			diffRanges.push_back(diff);
			return diffRanges;
		}

		return std::vector<Range> {inputRange};
	}
};

std::ostream& operator<<(std::ostream& os, const Range& range) {
	os << "start: " << range.start << ", end: " << range.end<<'\n';
	return os;
}

class Transformer {
	Range inRange, outRange;

public:
	Transformer(Range inRange, Range outRange) : inRange(inRange), outRange(outRange) {};


	//transform!
	std::vector<Range> Transform(std::vector<Range> inputRanges, std::vector<Range>& leftOvers) {

		std::vector<Range> ranges;
		for (auto& input : inputRanges) {
			if (inRange.IsIntersection(input)) {
				Range intersection = inRange.Intersect(input);
				long long delta = intersection.start - inRange.start;
				long long length = intersection.end - intersection.start + 1;
				ranges.push_back(Range(outRange.start + delta, outRange.start + delta + length -1));
			}
			std::vector<Range> differences = inRange.Difference(input);
			for (auto& diff : differences) {
				leftOvers.push_back(diff);
			}
		}
		return ranges;

	}

};

//represents a group of transforms from one thing to another (seed to soil, etc.)
class MapperProcess {
public:
	std::vector<Transformer> transforms;

	std::vector<Range> MapIt(std::vector<Range> inputRanges) {
		std::vector<Range> resultRanges;
		for (auto& transformer : transforms) {
			std::vector<Range> leftOvers;
			std::vector<Range> transformed;
			transformed = transformer.Transform(inputRanges, leftOvers);			
			
			for (auto& range : transformed) {
				resultRanges.push_back(range);
			}
			inputRanges = leftOvers;
		}
		for (auto& leftOver : inputRanges) {
			resultRanges.push_back(leftOver);
		}
		return resultRanges;		
	}
};

class SeedToLocationConverteroTron {
public:
	std::vector<MapperProcess> mappers;

	std::vector<Range> SeedToLocation(Range input) {
		std::vector<Range> inputAsVector{ input };
		
		for (auto& mapper : mappers) {
			inputAsVector = mapper.MapIt(inputAsVector);
		}
		
		return inputAsVector;
	}
};

int main() {

	std::ifstream ifs("inputs/day5/input.txt", std::ifstream::in);
	std::string s;

	SeedToLocationConverteroTron seedToLocationConverter;
	std::vector<Range> seedRanges;
	//part1
	//std::vector<long long> seeds;

	std::getline(ifs, s);
	std::regex seedRangePattern{ R"( (\d+) (\d+))" };
	std::regex transformPattern{ R"((\d+) (\d+) (\d+))" };
	std::regex mapPattern{ R"(map:$)" };
	
	std::vector<int> captureGroups = { 1,2 };
	std::sregex_token_iterator iterator(s.begin(), s.end(), seedRangePattern, {1,2});
	std::sregex_token_iterator iteratorEnd;

	while (iterator != iteratorEnd) {
		long long seed = std::stoll(*iterator);
		long long range = std::stoll(*(++iterator));
		seedRanges.push_back({ seed, seed + range - 1 });
		std::cout << "seed range start: " << seed <<", seed range end: "<< seed + range - 1 << '\n';
		iterator++;
	}

	while (ifs.good()) {		
		std::getline(ifs, s);
		
		if (s.empty()) {
			continue;
		}

		if (std::regex_search(s, mapPattern)) {
			MapperProcess mapProcess;

			while (ifs.good()) {
				std::getline(ifs, s);
				if (s.empty()) {
					break;
				}
				
				std::smatch smatch;

				bool matched = std::regex_search(s, smatch, transformPattern);
				
				long long out = std::stoll(smatch[1]);
				long long in = std::stoll(smatch[2]);
				long long range = std::stoll(smatch[3]);
				Range outRange(out, out + range - 1);
				Range inRange(in, in + range - 1);
				Transformer transformer(inRange, outRange);
				mapProcess.transforms.push_back(transformer);
			}
			seedToLocationConverter.mappers.push_back(mapProcess);
		}
	}

	ifs.close();

	//part 1
	/*long long minLocation = seedToLocationConverter.SeedToLocation(seeds[0]);;
	for (auto& seed : seeds) {
		long long location = seedToLocationConverter.SeedToLocation(seed);
		if (location < minLocation) {
			minLocation = location;
		}
	}*/

	//std::cout << "min seed location: " << minLocation<<'\n';

	//seed range 1 test
	long long minStart = std::numeric_limits<long long>::max();
	for (auto& seedRange : seedRanges) {
		std::vector<Range> locationRanges = seedToLocationConverter.SeedToLocation(seedRange);
		for (auto& locationRange : locationRanges) {
			minStart = std::min(locationRange.start, minStart);
		}
	}
	
	std::cout << "min start:" << minStart<< '\n';
	

	return 0;
}