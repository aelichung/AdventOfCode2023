#include<iostream>
#include<sstream>
#include<fstream>
#include<regex>
#include<numeric>
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
};

class PositionHash {
public:
	size_t operator()(const Position& pos) const
	{
		return (std::hash<long long>()(pos.x)) ^ (std::hash<long long>()(pos.y));
	}
};

class MapNode {	
public:
	Position position;
	char character;
	MapNode() {};
	MapNode(Position pos, char c) :position(pos), character(c) {};

	constexpr bool operator==(const MapNode& mapNode) {
		return mapNode.position.x == position.x && mapNode.position.y == position.y;
	}
};

class MapGrid {
public:
	MapNode start;
	long long rowCount, columnCount;
	std::vector<std::vector<MapNode>> nodes;
	MapGrid() {};
};

std::vector<MapNode> MakeRow(std::string s, long long rowCount, MapGrid& grid) {
	std::vector<MapNode> row;
	for (long long i = 0; i < s.size(); i++) {
		char c = s[i];
		Position pos(i, rowCount);
		MapNode node(pos, c);
		if (c == 'S') {
			grid.start = node;
		}
		row.push_back(node);
	}
	return row;
};

enum orientation {
	above,
	below,
	left,
	right
};

Position Move(Position source, Position position, char c, orientation& o) {
	Position destination = source;
	switch (c) {
	case '|':		// |	
		//if our source is above our pipe
		if (source.y < position.y) {
			destination.y += 2; //move down
			o = below;
		}
		else if (source.y > position.y) {
			destination.y -= 2; //move up
			o = above;
		}
		break;

	case '-': // -
		//if our source is to the west of our pipe
		if (source.x < position.x) {
			destination.x += 2;
			o = right;
		}
		else {
			destination.x -= 2;
			o = left;
		}
		break;

	case 'L': // L
		if (source.y < position.y && source.x == position.x) {
			destination.x += 1;
			destination.y += 1;
			o = right;
		}
		else if (source.x > position.x && source.y == position.y) {
			destination.x -= 1;
			destination.y -= 1;
			o = above;
		}
		else {
			std::cout << "invalid source passed to L Pipe" << std::endl;
		}
		break;

	case 'J': //J
		if (source.y < position.y && source.x == position.x) {
			destination.x -= 1;
			destination.y += 1;
			o = left;
		}
		else if (source.x < position.x && source.y == position.y) {
			destination.x += 1;
			destination.y -= 1;
			o = above;
		}
		else {
			std::cout << "invalid source passed to J Pipe" << std::endl;
		}
		break;

	case '7': //7
		if (source.y > position.y && source.x == position.x) {
			destination.x -= 1;
			destination.y -= 1;
			o = left;
		}
		else if (source.x < position.x && source.y == position.y) {
			destination.x += 1;
			destination.y += 1;
			o = below;
		}
		else {
			std::cout << "invalid source passed to 7 Pipe" << std::endl;
		}
		break;

	case 'F': //F
		if (source.y > position.y && source.x == position.x) {
			destination.x += 1;
			destination.y -= 1;
			o = right;
		}
		else if (source.x > position.x && source.y == position.y) {
			destination.x -= 1;
			destination.y += 1;
			o = below;
		}
		else {
			std::cout << "invalid source passed to F Pipe" << std::endl;
		}
		break;
	}
	return destination;
}


bool isValidConnection(char destinationType, char currentType, orientation o) {
	switch (currentType) {
	case '|':		// |	
		//not valid if 
		if (o == right || o == left) {
			return false;
		}
		if (destinationType == '-') {
			return false;
		}
		if (o == above) {
			if (destinationType == 'L' || destinationType == 'J') {
				return false;
			}
		}
		if (o == below) {
			if (destinationType == '7' || destinationType == 'F') {
				return false;
			}
		}
		return true;		

	case '-': // -
		//not valid if 
		if (o == above || o == below) {
			return false;
		}
		if (destinationType == '|') {
			return false;
		}
		if (o == left) {
			if (destinationType == '7' || destinationType == 'J') {
				return false;
			}
		}
		if (o == right) {
			if (destinationType == 'F' || destinationType == 'L') {
				return false;
			}
		}
		return true;

	case 'L': // L
		//not valid if 
		if (o == below || o == left) {
			return false;
		}
		if (o == above) {
			if (destinationType == 'L' || destinationType == 'J' || destinationType == '-') {
				return false;
			}
		}
		if (o == right) {
			if (destinationType == '|' || destinationType == 'F' || destinationType == 'L') {
				return false;
			}
		}
		return true;

	case 'J': //J
		//not valid if 
		if (o == below || o == right) {
			return false;
		}
		if (o == above) {
			if (destinationType == 'L' || destinationType == 'J' || destinationType == '-') {
				return false;
			}
		}
		if (o == left) {
			if (destinationType == '|' || destinationType == 'J' || destinationType == '7') {
				return false;
			}
		}
		return true;

	case '7': //7
		//not valid if 
		if (o == above || o == right) {
			return false;
		}
		if (o == below) {
			if (destinationType == '7' || destinationType == 'F' || destinationType == '-') {
				return false;
			}
		}
		if (o == left) {
			if (destinationType == '|' || destinationType == 'J' || destinationType == '7') {
				return false;
			}
		}
		return true;

	case 'F': //F
		//not valid if 
		if (o == above || o == left) {
			return false;
		}
		if (o == below) {
			if (destinationType == '7' || destinationType == 'F' || destinationType == '-') {
				return false;
			}
		}
		if (o == right) {
			if (destinationType == '|' || destinationType == 'F' || destinationType == 'L') {
				return false;
			}
		}
		return true;
	}
	if (destinationType == 'S') {
		return true;
	}
	return false;
}

int main() {
	std::ifstream ifs("input.txt");
	std::string inputString;	
	MapGrid grid;

	std::getline(ifs, inputString);
	grid.columnCount = inputString.size();
	long long rowCount = 0;	
	grid.nodes.push_back(MakeRow(inputString, rowCount, grid));

	while (ifs.good()) {
		++rowCount;
		std::getline(ifs, inputString);
		grid.nodes.push_back(MakeRow(inputString, rowCount, grid));
	}
	ifs.close();

	grid.rowCount = rowCount + 1;
	std::cout << "grid row count: " << grid.rowCount << "grid.column count:" << grid.columnCount << std::endl;

	// go to the start:
	Position startPos = grid.start.position;

	//find the adjacent connection pipes
	std::vector<std::vector<MapNode>> possibleLoops;
	if (startPos.x > 0) {
		//check the node on the left of our start
		std::vector<MapNode> leftLoop;
		leftLoop.push_back(grid.start);
		MapNode left = grid.nodes.at(startPos.y).at(startPos.x - 1);
		switch (left.character) {
			case '-':
				if (left.position.x > 0) {
					leftLoop.push_back(left);
				}
				break;
			case 'L':
				if (left.position.y > 0) {
					leftLoop.push_back(left);
				}
				break;
			case 'F':
				if (left.position.y < grid.rowCount -1) {
					leftLoop.push_back(left);
				}
				break;
		}
		if (leftLoop.size() > 1) {
			possibleLoops.push_back(leftLoop);
		}
	}
	//check the right side of start
	if (startPos.x < grid.columnCount - 1) {
		std::vector<MapNode> rightLoop;
		rightLoop.push_back(grid.start);
		MapNode right = grid.nodes.at(startPos.y).at(startPos.x + 1);
		switch (right.character) {
		case '-':
			if (right.position.x < grid.columnCount - 1) {
				rightLoop.push_back(right);
			}
			break;
		case 'J':
			if (right.position.y > 0) {
				rightLoop.push_back(right);
			}
			break;
		case '7':
			if (right.position.y < grid.rowCount -1) {
				rightLoop.push_back(right);
			}
			break;
		}
		if (rightLoop.size() > 1) {
			possibleLoops.push_back(rightLoop);
		}
	}

	//check above the start
	if (startPos.y > 0) {
		std::vector<MapNode> aboveLoop;
		aboveLoop.push_back(grid.start);
		MapNode above = grid.nodes.at(startPos.y - 1).at(startPos.x);
		switch (above.character) {
		case '|':
			if (above.position.y > 0) {
				aboveLoop.push_back(above);
			}
			break;
		case 'F':
			if (above.position.x < grid.columnCount - 1) {
				aboveLoop.push_back(above);
			}
			break;
		case '7':
			if (above.position.x > 0) {
				aboveLoop.push_back(above);
			}
			break;
		}
		if (aboveLoop.size() > 1) {
			possibleLoops.push_back(aboveLoop);
		}
	}

	//check below the start
	if (startPos.y < grid.rowCount - 1) {
		std::vector<MapNode> belowLoop;
		belowLoop.push_back(grid.start);
		MapNode below = grid.nodes.at(startPos.y + 1).at(startPos.x);
		switch (below.character) {
		case '|':
			if (below.position.y < grid.rowCount - 1) {
				belowLoop.push_back(below);
			}
			break;
		case 'L':
			if (below.position.x < grid.columnCount - 1) {
				belowLoop.push_back(below);
			}
			break;
		case 'J':
			if (below.position.x > 0) {
				belowLoop.push_back(below);
			}
			break;
		}
		if (belowLoop.size() > 1) {
			possibleLoops.push_back(belowLoop);
		}
	}

	//for each starter in possible loops, we want to move along the loop until we
	//A: return to start (we found our loop!)
	//B: run into a dead end, this can be the edge of the grid or a pipe that doesn't connect
	std::vector<MapNode> actualLoop;
	for (auto& loop : possibleLoops) {
		MapNode currentNode = loop.back();
		MapNode previousNode = *(loop.end() - 2);
		bool isBlocked = false;

		while (!isBlocked && currentNode.character != 'S')
		{
			MapNode currentNode = loop.back();
			MapNode previousNode = *(loop.end() - 2);
			orientation o;
			Position pos = Move(previousNode.position, currentNode.position, currentNode.character, o);
			if (pos.x >= 0 && pos.x < grid.columnCount && pos.y >= 0 && pos.y < grid.rowCount) {
				MapNode next = grid.nodes.at(pos.y).at(pos.x);
				if (next.character == '.') {
					isBlocked = true;
					continue;
				}
				if (!isValidConnection(next.character, currentNode.character, o)) {
					isBlocked = true;
					continue;
				}
				loop.push_back(next);
			}
			else {
				//went past the grid edge
				isBlocked = true;
				continue;
			}			
		}
		
		if (loop.back().character == 'S') {
			actualLoop = loop;
			break;
		}
	}
	std::unordered_set<Position, PositionHash> loopPositions;
	for (auto& node : actualLoop) {
		std::cout << "node: "<<node.character << std::endl;
		loopPositions.insert(node.position);
	}

	long long furthestDistance = (actualLoop.size() - 1) / 2;
	std::cout << "number of steps furthest from start: " << furthestDistance << std::endl;

	//figure out what S is
	MapNode startConnection1 = actualLoop.at(1);
	MapNode startConnection2 = actualLoop.at(actualLoop.size() - 2);

	if (startConnection1.position.y == grid.start.position.y && startConnection2.position.y == grid.start.position.y) //F,7,J,L,|,-
	{
		grid.start.character = '-';		
	}

	if (startConnection1.position.x == grid.start.position.x && startConnection2.position.x == grid.start.position.x)
	{
		grid.start.character = '|';
	}

	if (startConnection1.position.x > grid.start.position.x) //start is F or L
	{
		if (startConnection2.position.y > grid.start.position.y) {
			grid.start.character = 'F';
		}
		else {
			grid.start.character = 'L';
		}
	}

	//vice versa?
	if (startConnection2.position.x > grid.start.position.x) //start is F or L
	{
		if (startConnection1.position.y > grid.start.position.y) {
			grid.start.character = 'F';
		}
		else {
			grid.start.character = 'L';
		}
	}

	if (startConnection1.position.x < grid.start.position.x) //start is 7 or J
	{
		if (startConnection2.position.y > grid.start.position.y) {
			grid.start.character = 'J';
		}
		else {
			grid.start.character = '7';
		}
	}

	if (startConnection2.position.x < grid.start.position.x) //start is 7 or J
	{
		if (startConnection1.position.y > grid.start.position.y) {
			grid.start.character = 'J';
		}
		else {
			grid.start.character = '7';
		}
	}

	grid.nodes.at(grid.start.position.y).at(grid.start.position.x).character = grid.start.character;
	long long groundSum = 0;

	for (long long rowIndex = 0; rowIndex < grid.rowCount; rowIndex++) {		
		std::vector<MapNode> row = grid.nodes[rowIndex];
		long long lastKnownLoopEdge = 0;
		int edgeEncounters = 0;
		bool encounteredF = false;
		bool encounteredL = false;
		for (long long colIndex = 0; colIndex < grid.columnCount; colIndex++) {
			MapNode node = row.at(colIndex);	
			long long diff = 0;
			
			if (loopPositions.contains(node.position)) {
				if (node.character == 'F') {
					encounteredF = true;
					diff = node.position.x - lastKnownLoopEdge;
					++edgeEncounters;
					if (diff <= 1) continue;
					if (edgeEncounters % 2 == 0 && edgeEncounters != 0) {
						groundSum -= diff - 1; //remove any ground area that was surrounded
					}
				}
				if (node.character == 'J' && encounteredF) {
					encounteredF = false;					
					lastKnownLoopEdge = node.position.x;
				}
				if (node.character == '7' && encounteredF) {
					encounteredF = false;
					++edgeEncounters;
					lastKnownLoopEdge = node.position.x;
				}
				if (node.character == 'L') {
					encounteredL = true;					
					++edgeEncounters;
					diff = node.position.x - lastKnownLoopEdge;
					if (diff <= 1) continue;
					if (edgeEncounters % 2 == 0 && edgeEncounters != 0) {
						groundSum -= diff - 1; //remove any ground area that was surrounded
					}
				}
				if (node.character == 'J' && encounteredL) {
					encounteredL = false;
					++edgeEncounters;
					lastKnownLoopEdge = node.position.x;
				}
				if (node.character == '7' && encounteredL) {
					encounteredL = false;					
					lastKnownLoopEdge = node.position.x;
				}
				if (node.character == '|') {
					++edgeEncounters;
					diff = node.position.x - lastKnownLoopEdge;
					lastKnownLoopEdge = node.position.x;
					if (diff <= 1) continue;
					if (edgeEncounters % 2 == 0 && edgeEncounters != 0) {
						groundSum -= diff - 1; //remove any ground area that was surrounded
					}
				}						
							
							
							
				continue;
			}
			
			++groundSum;
		}
		std::cout << "amount of outer ground area in the row number: " << rowIndex << " is:" << groundSum << std::endl;
	}
	
	long long totalArea = grid.rowCount * grid.columnCount;
	long long innerArea = totalArea - loopPositions.size() - groundSum;
	
	std::cout << "part 2, inner area: " << innerArea<<  std::endl;

	return 0;
}