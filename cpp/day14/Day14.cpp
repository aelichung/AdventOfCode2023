#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <unordered_set>

struct gridHash {
	std::size_t operator()(std::vector<std::string> const& grid) const {
		std::hash<std::string> hasher;
		std::string hashString;
		for (auto string : grid) {
			hashString += string;
		}
		return hasher(hashString);
	}
};

void TiltNorth(std::vector<std::string>& grid) {	
	std::string firstRow = grid[0];
	for (long long colIndex = 0; colIndex < firstRow.size(); colIndex++) 
	{
		long long maxUpperPosition = -1;
		for (long long rowIndex = 0; rowIndex < grid.size(); rowIndex++) {
			std::string row = grid[rowIndex];
			char c = row[colIndex];
			switch (c) {
			case 'O':
				//switch the characters around in the string Grid and 
				if (maxUpperPosition != -1 && maxUpperPosition != rowIndex) {
					grid[maxUpperPosition][colIndex] = 'O';
					grid[rowIndex][colIndex] = '.';
					++maxUpperPosition;
				}
				//we are already at the upmost position because it hasn't been set yet
				else {					
					maxUpperPosition = rowIndex + 1;
				}
				break;
			case '#':
				maxUpperPosition = rowIndex + 1;
				break;
			case '.':
				if (maxUpperPosition > rowIndex || maxUpperPosition == -1) {
					maxUpperPosition = rowIndex;
				}
				break;
			}
		}
	}
}

void TiltWest(std::vector<std::string>& grid) {	
	for (long long rowIndex = 0; rowIndex < grid.size(); rowIndex++) {
		long long maxWestPosition = -1;
		std::string row = grid[rowIndex];
		for (long long colIndex = 0; colIndex < row.size(); colIndex++) {
			char c = row[colIndex];
			switch (c) {
			case 'O':
				//switch the characters around in the string Grid and 
				if (maxWestPosition != -1 && maxWestPosition != colIndex) {
					grid[rowIndex][maxWestPosition] = 'O';
					grid[rowIndex][colIndex] = '.';
					++maxWestPosition;
				}
				//we are already at the upmost position because it hasn't been set yet
				else {
					maxWestPosition = colIndex + 1;
				}
				break;
			case '#':
				maxWestPosition = colIndex + 1;
				break;
			case '.':
				if (maxWestPosition > colIndex|| maxWestPosition == -1) {
					maxWestPosition = colIndex;
				}
				break;
			}
		}
	}
}

void TiltSouth(std::vector<std::string>& grid) {	
	std::string firstRow = grid[0];
	for (long long colIndex = 0; colIndex < firstRow.size(); colIndex++)
	{
		long long maxLowPosition = -1;
		for (long long rowIndex = grid.size() - 1; rowIndex >= 0; rowIndex--) {
			std::string row = grid[rowIndex];
			char c = row[colIndex];
			switch (c) {
			case 'O':
				//switch the characters around in the string Grid and 
				if (maxLowPosition != -1 && maxLowPosition != rowIndex) {
					grid[maxLowPosition][colIndex] = 'O';
					grid[rowIndex][colIndex] = '.';
					--maxLowPosition;
				}
				//we are already at the upmost position because it hasn't been set yet
				else {
					maxLowPosition = rowIndex - 1;
				}
				break;
			case '#':
				maxLowPosition = rowIndex - 1;
				break;
			case '.':
				if (maxLowPosition < rowIndex || maxLowPosition == -1) {
					maxLowPosition = rowIndex;
				}
				break;
			}
		}
	}
}

void TiltEast(std::vector<std::string>& grid) {
	for (long long rowIndex = 0; rowIndex < grid.size(); rowIndex++) {
		long long maxEastPosition = -1;
		std::string row = grid[rowIndex];
		for (long long colIndex = row.size() - 1; colIndex >= 0; colIndex--) {
			char c = row[colIndex];
			switch (c) {
			case 'O':
				//switch the characters around in the string Grid and 
				if (maxEastPosition != -1 && maxEastPosition != colIndex) {
					grid[rowIndex][maxEastPosition] = 'O';
					grid[rowIndex][colIndex] = '.';
					--maxEastPosition;
				}
				//we are already at the upmost position because it hasn't been set yet
				else {
					maxEastPosition = colIndex - 1;
				}
				break;
			case '#':
				maxEastPosition = colIndex - 1;
				break;
			case '.':
				if (maxEastPosition < colIndex || maxEastPosition == -1) {
					maxEastPosition = colIndex;
				}
				break;
			}
		}
	}
}

void Cycle(std::vector<std::string>& grid) {
	TiltNorth(grid);
	TiltWest(grid);
	TiltSouth(grid);
	TiltEast(grid);
}

int main() {

	std::ifstream ifs("input.txt");
	std::string s;	
	long long rowCount = 0;

	std::vector<std::string> strings;
	while (ifs.good()) {
		std::getline(ifs, s);		

		strings.push_back(s);

		rowCount++;
	}
	ifs.close();

	long long cycleCount = 1;
	std::map<std::vector<std::string>, long long> gridMap;
	std::map<long long, std::vector<std::string>> cycleMap;

	Cycle(strings);

	while (!gridMap.contains(strings)) {
		gridMap[strings] = cycleCount;
		cycleMap[cycleCount] = strings;

		Cycle(strings);
		++cycleCount;
	}

	//length of loop in number of cycles
	long long loopLength = cycleCount - gridMap[strings];
	long long remainderCycles = (1000000000 - gridMap[strings]) % loopLength;

	std::vector<std::string> billionthGrid;
	if (remainderCycles == 0) {
		billionthGrid = strings;
	}
	else {
		long long billionthCycle = remainderCycles + gridMap[strings];
		billionthGrid = cycleMap[billionthCycle];
	}

	long long loadSum = 0;
	long long upper = billionthGrid.size();
	//calculate load on billionth Grid
	for (long long rowIndex = 0; rowIndex < billionthGrid.size(); rowIndex++) {		
		std::string row = billionthGrid[rowIndex];
		for (long long colIndex = 0; colIndex < row.size(); colIndex++) {
			char c = row[colIndex];
			if (c == 'O') {
				loadSum += upper - rowIndex;
			}
		}
	}


	std::cout << "sum: " << loadSum << "\n";
	return 0;
}