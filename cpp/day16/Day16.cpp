#include<iostream>
#include<sstream>
#include<fstream>
#include <unordered_map>


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
};

class PositionHash {
public:
	size_t operator()(const Position& pos) const
	{
		return (std::hash<long long>()(pos.x)) ^ (std::hash<long long>()(pos.y));
	}
};

struct PositionData {
public:
	char character;
	bool visitedRight = false, visitedLeft = false, visitedUp = false, visitedDown = false;
	bool visited = false;

	PositionData() {
		character = 'f';
	}
	PositionData(char c) : character(c) {}
};

enum Direction {
	up, right, left, down
};


bool isWithinBounds(Position position, long long rowCount, long long columnCount) {
	long long x = position.x;
	long long y = position.y;
	return (x >= 0 && x < columnCount) && (y >= 0 && y < rowCount);
}


/*
void Visit(std::unordered_map<Position, PositionData, PositionHash>& map, long long& visitedCount, Position currentPos, long long rowCount, long long columnCount, Direction direction) {
	PositionData& data = map[currentPos];
	if (!data.visited) {
		++visitedCount;
		data.visited = true;
	}

	long long x = currentPos.x;
	long long y = currentPos.y;

	switch (direction) {
	case up:
		--y;
		break;
	case right:
		++x;
		break;
	case left:
		--x;
		break;
	case down:
		++y;
		break;
	}

	Position next(x, y);

	MoveNext(map, visitedCount, rowCount, columnCount, next, direction);
}
*/

void Visit(
	std::unordered_map<Position, PositionData, 
	PositionHash>& map, long long& visitedCount, 
	Position currentPosition, 
	long long rowCount, 
	long long columnCount,  
	Direction currentDirection) {

	Position nextPosition = currentPosition;
	Direction nextDirection = currentDirection;

	while (true) {
		PositionData& data = map[currentPosition];
		if (!isWithinBounds(currentPosition, rowCount, columnCount)) {
			return;
		}

		switch (currentDirection) {
		case up:
			if (data.visitedUp) return;
			data.visitedUp = true;
			break;
		case right:
			if (data.visitedRight) return;
			data.visitedRight = true;
			break;
		case left:
			if (data.visitedLeft) return;
			data.visitedLeft = true;
			break;
		case down:
			if (data.visitedDown) return;
			data.visitedDown = true;
			break;
		}
		
		if (!data.visited) {
			++visitedCount;
			data.visited = true;
			//std::cout << currentPosition.x << ", " << currentPosition.y << '\n';
		}


		switch (data.character) {
		case '.':
			//continue moving in same direction
			nextDirection = currentDirection;
			break;
		case '/':
			switch (currentDirection) {
			case up:
				nextDirection = right;
				break;
			case right:
				nextDirection = up;
				break;
			case left:
				nextDirection = down;
				break;
			case down:
				nextDirection = left;
				break;
			}
			break;

		case '-':
			if (currentDirection == right || currentDirection == left) {
				nextDirection = currentDirection;
			}
			else {
				Position rightPosition = Position(currentPosition.x +1, currentPosition.y);
				//move right
				Visit(map, visitedCount, rightPosition, rowCount, columnCount, right);
				//move left
				Position leftPosition = Position(currentPosition.x - 1, currentPosition.y);
				Visit(map, visitedCount, leftPosition, rowCount, columnCount, left);
				return;
			}
			break;
		case '|':
			if (currentDirection == up || currentDirection == down) {
				nextDirection = currentDirection;
			}
			else {
				//move up
				Position upPosition = Position(currentPosition.x, currentPosition.y -1);
				Visit(map, visitedCount, upPosition, rowCount, columnCount, up);
				//move down
				Position downPosition = Position(currentPosition.x, currentPosition.y + 1);
				Visit(map, visitedCount, downPosition, rowCount, columnCount, down);
				return;
			}
			break;
		case '\\':
			switch (currentDirection) {
			case up:
				nextDirection = left;
				break;
			case right:
				nextDirection = down;
				break;
			case left:
				nextDirection = up;
				break;
			case down:
				nextDirection = right;
				break;
			}
			break;
		}
		currentDirection = nextDirection;
		long long x = currentPosition.x;
		long long y = currentPosition.y;

		switch (currentDirection) {
		case up:
			--y;
			break;
		case right:
			++x;
			break;
		case left:
			--x;
			break;
		case down:
			++y;
			break;
		}

		Position nextPosition = Position(x, y);
		currentPosition = nextPosition;
		
	}

	//Visit(map, visitedCount, currentPosition, rowCount, columnCount, nextDirection);
}

int main() {
	std::unordered_map<Position, PositionData, PositionHash> visitedMap;
	std::unordered_map<Position, PositionData, PositionHash> initialMap;
	long long visitedCount = 0;

	std::ifstream ifs("input.txt");
	std::string s;
	long long rowIndex = 0;
	while (ifs.good()) {
		std::getline(ifs, s);

		for (long long colIndex = 0; colIndex < s.length(); colIndex++) {
			Position pos(colIndex, rowIndex);
			PositionData data(s[colIndex]);
			std::pair<Position, PositionData> item(pos, data);
			visitedMap.insert(item);
		}
		++rowIndex;
	}
	long long colCount = s.length();
	ifs.close();
	initialMap = visitedMap;

	/*Position start(0, 0);
	Visit(visitedMap, visitedCount, start, rowIndex, colCount, right);

	std::cout << " visited count: " << visitedCount << '\n';*/

	std::vector<Position> upperEdgeStarts;
	std::vector<Position> lowerEdgeStarts;
	std::vector<Position> rightEdgeStarts;
	std::vector<Position> leftEdgeStarts;
	for (long long i = 0; i < rowIndex; i++) {
		Position leftPos(0, i);
		Position rightPos(colCount - 1, i);
		leftEdgeStarts.push_back(leftPos);
		rightEdgeStarts.push_back(rightPos);
	}

	for (long long i = 0; i < colCount; i++) {
		Position upPos(i, 0);
		Position downPos( i, rowIndex - 1 );
		upperEdgeStarts.push_back(upPos);
		lowerEdgeStarts.push_back(downPos);
	}	

	long long maxVisisted = 0;
	for (auto pos : upperEdgeStarts) {
		visitedCount = 0;
		visitedMap = initialMap;
		Visit(visitedMap, visitedCount, pos, rowIndex, colCount, down);
		if (visitedCount > maxVisisted) {
			maxVisisted = visitedCount;
		}
	}

	for (auto pos : lowerEdgeStarts) {
		visitedCount = 0;
		visitedMap = initialMap;
		Visit(visitedMap, visitedCount, pos, rowIndex, colCount, up);
		if (visitedCount > maxVisisted) {
			maxVisisted = visitedCount;
		}
	}

	for (auto pos : leftEdgeStarts) {
		visitedCount = 0;
		visitedMap = initialMap;
		Visit(visitedMap, visitedCount, pos, rowIndex, colCount, right);
		if (visitedCount > maxVisisted) {
			maxVisisted = visitedCount;
		}
	}

	for (auto pos : rightEdgeStarts) {
		visitedCount = 0;
		visitedMap = initialMap;
		Visit(visitedMap, visitedCount, pos, rowIndex, colCount, left);
		if (visitedCount > maxVisisted) {
			maxVisisted = visitedCount;
		}
	}

	std::cout << "max visited: " << maxVisisted << '\n';
	return 0;
}
