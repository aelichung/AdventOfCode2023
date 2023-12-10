#include<iostream>
#include<sstream>
#include<fstream>
#include<regex>
#include<numeric>

//class DiffValue {
//	long long &value,  &left, &right;
//	DiffValue(long long& val, long long& leftInput, long long& rightInput) : value(val), left(leftInput), right(rightInput) {};
//};

class ValueHistory {
	std::vector<long long> Values;
	std::vector<std::vector<long long>> differencePyramid;

	std::vector<std::vector<long long>> BuildPyramid(std::vector<long long> values) {
		std::vector<std::vector<long long>> diffPyramid;
		//while the differences are not all 0's, we should make a new vector and populate it with differences
		std::vector<long long> differences = values;
		bool AllDifferencesAreZero = false;
		while (!AllDifferencesAreZero) {
			diffPyramid.push_back(differences);
			int numZeros = 0;
			for (auto& diff : differences) {
				if (diff != 0) {
					break;
				}
				else {
					++numZeros;
				}
			}
			if (numZeros == differences.size()) {
				AllDifferencesAreZero = true;
				break;
			}

			std::vector<long long> tempDiffs;
			for (int diffIndex = 1; diffIndex < differences.size(); diffIndex++) {
				long long difference = differences[diffIndex] - differences[diffIndex - 1];
				tempDiffs.push_back(difference);
			}
			differences = tempDiffs;
		}
		return diffPyramid;
	}
public:	
	ValueHistory(std::vector<long long> values) : Values(values) {
		differencePyramid = BuildPyramid(values);
	};

	long long GetFuturePrediction() {
		long long add = 0;
		differencePyramid.back().push_back(0ll);
		auto pyramidIter = differencePyramid.rbegin();
		while (std::next(pyramidIter,1) != differencePyramid.rend()) {
			auto currentDiffVector = *pyramidIter;
			auto aboveDiffVector = *std::next(pyramidIter, 1);
			add = aboveDiffVector.back() + currentDiffVector.back();
			aboveDiffVector.push_back(aboveDiffVector.back() + currentDiffVector.back());
			*std::next(pyramidIter, 1) = aboveDiffVector;
			++pyramidIter;			
		}
		return add;
	}

	long long GetPastPrediction() {
		long long add = 0;
		differencePyramid.back().push_back(0ll);
		auto pyramidIter = differencePyramid.rbegin();
		while (std::next(pyramidIter, 1) != differencePyramid.rend()) {
			auto currentDiffVector = *pyramidIter;
			auto aboveDiffVector = *std::next(pyramidIter, 1);
			add = aboveDiffVector.front() - currentDiffVector.front();
			aboveDiffVector.insert(aboveDiffVector.begin(), add);
			*std::next(pyramidIter, 1) = aboveDiffVector;
			++pyramidIter;
		}
		return add;
	}

};

int main() {
	std::ifstream ifs("input.txt");
	std::string inputString;
	std::regex numberPattern{ R"((-?\d+))" };

	std::vector<ValueHistory> histories;

	while (ifs.good()) {
		std::getline(ifs, inputString);
		std::sregex_token_iterator regexIter(inputString.begin(), inputString.end(), numberPattern, 1);
		std::sregex_token_iterator regexEnd;

		std::vector<long long> historyValues;
		while (regexIter != regexEnd) {
			historyValues.push_back(std::stoll(*regexIter));
			regexIter++;
		}
		ValueHistory history(historyValues);
		histories.push_back(history);
	}
	ifs.close();

	long long futureSum = 0;
	long long pastSum = 0;

	for (auto& history : histories) {
		auto prediction = history.GetFuturePrediction();
		futureSum += prediction;
		
		auto pastPrediction = history.GetPastPrediction();
		std::cout<<"past prediction: "<<pastPrediction << std::endl;
		pastSum += pastPrediction;
	}
	std::cout << "part 1 future prediction sum: " << futureSum << std::endl;

	std::cout << "part 2 past prediction sum: "<< pastSum << std::endl;

	return 0;
}