#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main() {

	std::ifstream ifs("input.txt");
	std::string s;
	std::vector<std::string> horizontalStrings;

	while (ifs.good()) {
		std::getline(ifs, s);
		horizontalStrings.push_back(s);
	}

	std::vector<std::string> verticalStrings;

	for (long long charIndex = 0; charIndex < horizontalStrings.at(0).length(); charIndex++) {
		std::string vString;
		for (long long rowIndex = 0; rowIndex < horizontalStrings.size(); rowIndex++) {
			char c = horizontalStrings.at(rowIndex).at(charIndex);
			vString += c;
		}
		verticalStrings.push_back(vString);
	}
	
	long long upperRow = verticalStrings.at(0).size();
	long long sum = 0;

	for (long long i = 0; i < verticalStrings.size(); i++) {
		long long maxUpperPosition = -1;
		std::string columnString = verticalStrings[i];
		for (long long j = 0; j < columnString.size(); j++) {
			char c = columnString[j];
			switch (c) {
			case 'O':
				//add the upmost free position to the sum
				if (maxUpperPosition != -1) {
					sum += upperRow - maxUpperPosition;
					++maxUpperPosition;
				}
				//we are already at the upmost position because it hasn't been set yet, add the current row position to sum
				else {
					sum += upperRow - j;
					maxUpperPosition = j + 1;
				}
				break;
			case '.':
				if (maxUpperPosition > j || maxUpperPosition == -1) {
					maxUpperPosition = j;
				}
				break;
			case '#':
				maxUpperPosition = j + 1;
				break;
			}
		}
	}

	ifs.close();

	std::cout << "sum: " << sum << "\n";
	return 0;
}