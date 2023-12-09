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
public:	
	ValueHistory(std::vector<long long> values) : Values(values) {};

	long long GetPrediction() {
		std::vector<std::vector<long long>> differencePyramid;
		//while the differences are not all 0's, we should make a new vector and populate it with differences
		std::vector<long long> differences = Values;
		bool AllDifferencesAreZero = false;
		while (!AllDifferencesAreZero) {
			differencePyramid.push_back(differences);
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

		long long add = 0;
		differencePyramid.back().push_back(0ll);
		auto pyramidIter = differencePyramid.rbegin() + 1;
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

	long long sum = 0;

	for (auto& history : histories) {
		auto prediction = history.GetPrediction();
		sum += prediction;
		std::cout << "prediction: " << prediction << std::endl;
	}
	std::cout << "sum: " << sum << std::endl;

	return 0;
}