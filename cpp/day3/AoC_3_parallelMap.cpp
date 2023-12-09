#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main() {	
	std::ifstream ifs("testInput_day3.txt", std::ifstream::in);

	std::string s;	
	std::vector<std::vector<char>> letterGrid;
	std::vector<std::vector<bool>> dirtyGrid;

	while (ifs.good()) {
		std::getline(ifs, s);
		std::vector<char> rowLetterVector;
		std::vector<bool> rowBoolVector;

		for (int i = 0; i < s.length(); i++) {
			char c = s[i];
			rowLetterVector.push_back(c);
			rowBoolVector.push_back(false);
		}

		letterGrid.push_back(rowLetterVector);
		dirtyGrid.push_back(rowBoolVector);
	}
	ifs.close();
	
	for (int rowIndex = 0; rowIndex < letterGrid.size(); rowIndex++) {
		for (int colIndex = 0; colIndex < letterGrid[rowIndex].size(); colIndex++) {
			char c = letterGrid.at(rowIndex).at(colIndex);
			if (c == '.') continue;
			
		}
	}

	std::cout << letterGrid.at(0).at(0);

	return 0;
}