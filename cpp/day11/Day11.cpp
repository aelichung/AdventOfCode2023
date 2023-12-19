#include<iostream>
#include<sstream>
#include<fstream>
#include <unordered_set>

struct Position {
public:
	long long x, y; //where x is the column count and y is the row count
	Position() {
		x = -1;
		y = -1;
	};
	Position(long long x, long long y) : x(x), y(y) {};

	bool operator==(const Position& position) const
	{
		return position.x == x && position.y == y;
	}

	long long MinDistanceBetween(const Position position) {
		long long xDiff, yDiff;
		xDiff = std::abs(x - position.x);
		yDiff = std::abs(y - position.y);
		return xDiff + yDiff;
	}
};

std::ostream& operator<<(std::ostream& os, const Position& position) {
	os << "Position " << "( " << position.x << ", " << position.y << ")" << std::endl;
	return os;
}

class PositionHash {
public:
	size_t operator()(const Position& pos) const
	{
		return (std::hash<long long>()(pos.x)) ^ (std::hash<long long>()(pos.y));
	}
};


struct Universe {
public:	
	const long long expansionFactor = 999999;
	std::unordered_set<Position, PositionHash> collapsedGalaxies;
	std::vector<Position> expandedGalaxies;	
	Universe() {};

	void Expand(long long rowCount, long long columnCount) {
		std::vector<long long> emptyRows, emptyColumns;

		for (long long rowIndex = 0; rowIndex < rowCount; rowIndex++) {
			bool rowInhabited = false;
			for (long long colIndex = 0; colIndex < columnCount; colIndex++) {
				if (collapsedGalaxies.contains(Position(colIndex, rowIndex))) {
					rowInhabited = true;
					break;
				}
			}
			if (!rowInhabited) {
				emptyRows.push_back(rowIndex);
			}
		}

		for (long long colIndex = 0; colIndex < columnCount; colIndex++)
		{
			bool colInhabited = false;
			for (long long rowIndex = 0; rowIndex < rowCount; rowIndex++) {
				if (collapsedGalaxies.contains(Position(colIndex, rowIndex))) {
					colInhabited = true;
					break;
				}
			}
			if (!colInhabited) {
				emptyColumns.push_back(colIndex);
			}
		}

		for (auto& galaxy : collapsedGalaxies) {
			expandedGalaxies.push_back(galaxy);		
		}

		for (auto& galaxy : expandedGalaxies) {
			Position oldPos = galaxy;
			for (auto& colIndex : emptyColumns)
			{
				if (oldPos.x > colIndex) {
					galaxy.x += expansionFactor;
				}
			}
			for (auto& rowIndex : emptyRows) {
				if (oldPos.y > rowIndex) {
					galaxy.y += expansionFactor;
				}
			}
		}
	}

	long long MinDistanceSum() {
		long long sum = 0;
		for (long long i = 0; i< expandedGalaxies.size(); i++) {
			Position galaxyA = expandedGalaxies.at(i);
			for (long long j = i + 1; j < expandedGalaxies.size(); j++) {
				Position galaxyB = expandedGalaxies.at(j);
				sum += galaxyA.MinDistanceBetween(galaxyB);
			}
		}
		return sum;
	}
};

int main() {
	std::ifstream ifs("input.txt");
	std::string inputString;
	Universe universe;
	
	std::unordered_set<Position, PositionHash> galaxies;
	long long rowIndex = 0, columnIndex = 0;
	//read the galaxy from input
	while (ifs.good())
	{		
		std::getline(ifs, inputString);
		
		for (long long colIndex = 0; colIndex < inputString.size(); colIndex++) {
			Position pos(colIndex, rowIndex);
			if (inputString[colIndex] == '#') {
				galaxies.insert(pos);
			}
		}
		
		columnIndex = inputString.size();
		++rowIndex;
	}
	ifs.close();	
	
	universe.collapsedGalaxies = galaxies;
	
	//expand the galaxy
	universe.Expand(rowIndex, columnIndex);
	
	long long sum = universe.MinDistanceSum();
	std::cout << "the sum of minimum distances for all the galaxies is: " << sum << std::endl;
	return 0;
}