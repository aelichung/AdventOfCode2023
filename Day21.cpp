#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <memory>
 

int MAXSTEPS = 64;
struct Position {
	int x, y;

	bool operator== (const Position& p) const
	{
		return x == p.x && y == p.y;
	}
	struct HashFunction
	{
		size_t operator()(const Position& p) const
		{
			size_t xHash = std::hash<int>()(p.x);
			size_t yHash = std::hash<int>()(p.y) << 1;
			return xHash ^ yHash;
		}
	};
};

struct stepGroup {
	Position position;
	int stepsRemaining;
	stepGroup(Position position, int stepsRemaining) : position(position), stepsRemaining(stepsRemaining) {}

	bool operator== (const stepGroup& s) const
	{
		return position.x == s.position.x && position.y == s.position.y && stepsRemaining == s.stepsRemaining;
	}
	struct HashFunction
	{
		size_t operator()(const stepGroup& s) const
		{
			size_t xHash = std::hash<int>()(s.position.x);
			size_t yHash = std::hash<int>()(s.position.y) << 1;
			size_t stepHash = std::hash<int>()(s.stepsRemaining);
			return xHash ^ yHash ^ stepHash;
		}
	};
};

std::queue<stepGroup> stepGroupQueue;


int FindNextStep(std::vector<std::string> &grid, Position position) {	
	std::unordered_set<Position, Position::HashFunction> answers;
	std::unordered_set<stepGroup, stepGroup::HashFunction> seen;
	stepGroup stepGrp{ position,0 };
	stepGroupQueue.push(stepGrp);	

	while (!stepGroupQueue.empty()) {
		stepGroup stepData = stepGroupQueue.front();
		Position nextPos = stepData.position;
		int count = stepData.stepsRemaining;
		stepGroupQueue.pop();

		if (seen.find(stepData) != seen.end()) {
			continue;
		}

		if (count > MAXSTEPS) {
			continue;
		}

		if (count == MAXSTEPS) {
			answers.insert(nextPos);	
		}

		seen.insert(stepData);
		

		//if (count % 2 == 0) {
		//	answers.insert(nextPos);
		//}

		//go up
		if (nextPos.y > 0)
		{
			char upC = grid[nextPos.y - 1][nextPos.x];
			if (upC != '#') {
				Position pos{ nextPos.x, nextPos.y - 1 };
				stepGroup sgp{ pos, count + 1 };
				stepGroupQueue.push(sgp);
			}
		}

		//go right
		if (nextPos.x < grid[0].length() - 1)
		{
			char rightC = grid[nextPos.y][nextPos.x + 1];
			if (rightC != '#') {				
				Position pos{ nextPos.x + 1, nextPos.y};
				stepGroup sgp{ pos, count + 1 };
				stepGroupQueue.push(sgp);
			}
		}

		//go down
		if (nextPos.y < grid.size() - 1)
		{
			char downC = grid[nextPos.y + 1][nextPos.x];
			if (downC != '#') {				
				Position pos{ nextPos.x, nextPos.y  + 1};
				stepGroup sgp{ pos, count + 1 };
				stepGroupQueue.push(sgp);
			}
		}

		//go left
		if (nextPos.x > 0)
		{
			char leftC = grid[nextPos.y][nextPos.x - 1];
			if (leftC != '#') {				
				Position pos{ nextPos.x - 1, nextPos.y };
				stepGroup sgp{ pos, count + 1 };
				stepGroupQueue.push(sgp);
			}
		}
	}

	return answers.size();
}

int main() {
	std::vector<std::string> grid;
	std::ifstream ifs("input.txt");
	std::string input;
	Position start;
	int rowIndex = 0;
	while (ifs.good()) {

		std::getline(ifs, input);
		grid.push_back(input);
		auto found = input.find('S');
		if (found != std::string::npos) {
			start = { (int)found, rowIndex };
		}
		++rowIndex;
	}

	for (auto input : grid) {
		std::cout << input << '\n';
	}

	std::cout << "number of possible ending steps: " << FindNextStep(grid, start) << '\n';


	//part 2

	return 0;
}
