#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

class Position {
public:
	int Column, Row;
	Position(int row, int column) : Row(row),Column(column){}
	void Print() {
		std::cout << "Row: " << Row << ", Column: " << Column;
	}

	bool Equals(Position position) {
		if (position.Row == Row && position.Column == Column) {
			return true;
		}
		return false;
	}

	constexpr bool operator==(const Position& position) {
		return position.Row == Row && position.Column == Column;
	}
};

std::ostream& operator<<(std::ostream& os, const Position& pos) {
	os << "Row: " << pos.Row << ", Column: " << pos.Column;
	return os;
}


class NumPosition : public Position {
public:
	bool hasNeighbor;
	int Length, Value;

	NumPosition(int row, int column, int length, int value) : Position(row, column) {
		Length = length;
		Value = value;
		hasNeighbor = false;
	}

	void Print() {
		Position::Print();
		std::cout << ", Length: " << Length << ", Value: " << Value << '\n';
	}

	std::vector<Position> GetNeighborPositions() {
		std::vector<Position> neighborPositions;

		for (int rowIndex = Row - 1; rowIndex <= Row + 1; rowIndex++) {
			for (int colIndex = Column - 1; colIndex <= Column + Length; colIndex++) {
				if (rowIndex == Row && colIndex == Column) continue;
				Position pos(rowIndex, colIndex);
				neighborPositions.push_back(pos);
			}
		}

		return neighborPositions;
	}

	bool isCollision(Position collider) {
		for (int i = Column; i < Column + Length; i++) {
			Position pos(Row, i);
			if (collider.Equals(pos)) {							
				return true;
			}
		}
		return false;
	}
};

class GearPosition : public Position {
public:
	std::vector<NumPosition> parts;	
	explicit GearPosition(int row, int column) :  Position(row, column) {	};


	std::vector<Position> GetNeighborPositions() {
		std::vector<Position> neighborPositions;

		for (int rowIndex = Row - 1; rowIndex <= Row + 1; rowIndex++) {
			for (int colIndex = Column - 1; colIndex <= Column + 1; colIndex++) {
				if (rowIndex == Row && colIndex == Column) continue;
				Position pos(rowIndex, colIndex);
				neighborPositions.push_back(pos);
			}
		}

		return neighborPositions;
	}

	bool isValidGear() {
		std::cout << "Parts for gear: " << '\n';
		for (auto& part : parts) {
			part.Print();
		}
		std::cout << '\n';
		return parts.size() == 2;
	}

	int gearRatio() {
		int product = 1;
		for (auto& part : parts) {
			product *= part.Value;
		}
		return product;
	}
};




int main() {	
	std::ifstream ifs("input_day3.txt", std::ifstream::in);
	std::string s;	
	int columnCount = 0;
	int rowCount = 0;
	std::vector<Position> symbolPositions;
	std::vector<NumPosition> numberPositions;
	std::vector<GearPosition> gearPositions; //may or may not be a valid gear
	
	while (ifs.good()) {
		std::getline(ifs, s);		
		columnCount = s.length();

		/*std::string numString;
		int startIndex = 0;*/

		for (int i = 0; i < columnCount; i++) {
			char c = s[i];

			/*
			if (std::isdigit(c)) {
				if (numString.empty()) {
					startIndex = i;
				}
				numString += c;
				continue;
			}

			if (!numString.empty()) {
				int num = std::stoi(numString);
				NumPosition numPos(rowCount, startIndex, numString.length(), num);
				numberPositions.push_back(numPos);
				numString.clear();
			}
			*/


			//check if c is a symbol
			if (c == '.') {
				continue;
			}
			if (!std::isdigit(c)) {				
				Position pos{ rowCount,i };
				symbolPositions.push_back(pos);

				if (c == '*') {
					GearPosition gear{ rowCount, i };
					gearPositions.push_back(gear);
				}
			}
			else {
				std::string numString;
				
				for (int j = i; std::isdigit(s[j]); j++) {
					numString += s[j];
					i++;
				}

				int startIndex = i;
				while (std::isdigit(c)) {
					numString += c;
					i++;
					c = s[i];
				}
				i--;
				int num = std::stoi(numString);
				NumPosition numPos(rowCount, startIndex, numString.length(), num);
				numberPositions.push_back(numPos);
			}

		}
		rowCount++;
	}
	
	int sum = 0;
	int gearRatioSum = 0;
	for (auto& numPosition : numberPositions) {
		std::vector<Position> neighbors = numPosition.GetNeighborPositions();

		for (auto& neighborPosition : neighbors) {		

			for (auto& symbolPosition : symbolPositions) {
				if (symbolPosition.Equals(neighborPosition)) {
					numPosition.hasNeighbor = true;
					//std::cout << "number: " << numPosition.Value << ", has neighbor at row: " << symbolPosition.Row << ", column: " << symbolPosition.Column << '\n';
					break;
				}
			}
		}		
		if (numPosition.hasNeighbor) {
			
			sum += numPosition.Value;
		}
	}

	//std::cout << "sum: " << sum<<'\n';
	


	for (auto& gear : gearPositions) {
		std::vector<Position> neighbors = gear.GetNeighborPositions();
		for (auto& neighbor : neighbors) {
			for (auto& numPos : numberPositions) {
				if (numPos.isCollision(neighbor)) {
					//if the gear part is not already in the parts vector
					//we add it to the vector
					bool found = false;
					for (auto& part : gear.parts) {
					
						if (part.Equals(numPos)) {							
							found = true;
						}
					}
					if (!found) {
						gear.parts.push_back(numPos);
					}
					
				}
			}
		}
		if (gear.isValidGear()) {
			gearRatioSum += gear.gearRatio();
		}
	}

	std::cout << "gear ration sum: " << gearRatioSum;

	ifs.clear();
	return 0;
}