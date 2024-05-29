#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <memory>
#include <unordered_map>
 

int MAXSTEPS = 64;

struct Position {
	int x, y;

	bool operator== (const Position& p) const
	{
		return x == p.x && y == p.y;
	}
	bool operator<(const Position& p) const {
		return std::tie(x, y) < std::tie(p.x, p.y);
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



std::unordered_map<Position, int, Position::HashFunction> FindNextStep(std::vector<std::string> &grid, Position position) {
	stepGroup stepGrp{ position,0 };
	stepGroupQueue.push(stepGrp);	
	std::unordered_map<Position, int, Position::HashFunction> seen{};

	while (!stepGroupQueue.empty()) {
		stepGroup stepData = stepGroupQueue.front();
		Position nextPos = stepData.position;
		int count = stepData.stepsRemaining;
		stepGroupQueue.pop();

		if (seen.contains(nextPos)) {
			continue;
		}

		seen.insert(std::make_pair( nextPos, count ));
		
		//go up
		if (nextPos.y > 0)
		{
			char upC = grid[nextPos.y - 1][nextPos.x];
			Position pos{ nextPos.x, nextPos.y - 1 };
			stepGroup sgp{ pos, count + 1 };
			if (upC != '#' && !seen.contains(pos)) {								
				stepGroupQueue.push(sgp);
			}
		}

		//go right
		if (nextPos.x < grid[0].length() - 1)
		{
			char rightC = grid[nextPos.y][nextPos.x + 1];
			Position pos{ nextPos.x + 1, nextPos.y };
			stepGroup sgp{ pos, count + 1 };
			if (rightC != '#' && !seen.contains(pos)) {
				stepGroupQueue.push(sgp);
			}
		}

		//go down
		if (nextPos.y < grid.size() - 1)
		{
			char downC = grid[nextPos.y + 1][nextPos.x];
			Position pos{ nextPos.x, nextPos.y + 1 };
			stepGroup sgp{ pos, count + 1 };
			if (downC != '#' && !seen.contains(pos)) {
				stepGroupQueue.push(sgp);
			}
		}

		//go left
		if (nextPos.x > 0)
		{
			char leftC = grid[nextPos.y][nextPos.x - 1];
			Position pos{ nextPos.x - 1, nextPos.y };
			stepGroup sgp{ pos, count + 1 };
			if (leftC != '#' && !seen.contains(pos)) {
				stepGroupQueue.push(sgp);
			}
		}
	}

	return seen;
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

	auto seen = FindNextStep(grid, start);
	int part1plots = 0;
	for (auto& [position, steps] : seen) {
		if (steps <= 64 && steps % 2 == 0) {
			part1plots++;
		}
	}

	std::cout << "number of possible ending steps: " << part1plots << '\n';


	//part 2
	//using the formula from the fantastic explanation here: https://github.com/villuna/aoc23/wiki/A-Geometric-solution-to-advent-of-code-2023,-day-21
	// total = ((n+1)^2 * odd grids) + (n^2 * even grids) - (n+1) * odd corners + n * even corners

	int64_t MAXSTEPS_P2 = 26501365;

	int64_t even_corners = 0;
	int64_t odd_corners = 0;
	for (auto& [position, steps] : seen) {
		if (steps > 65 && steps % 2 == 0) {
			even_corners++;
		}
		if (steps > 65 && steps % 2 == 1) {
			odd_corners++;
		}
	}

	int64_t num_gardens_from_start = ((MAXSTEPS_P2 - (grid[0].length()) / 2) / grid[0].length());
	std::cout << "number of gardens is: " << num_gardens_from_start << '\n';

	int64_t even_full = 0;
	int64_t odd_full = 0;
	for (auto& [position, steps] : seen) {
		if (steps % 2 == 0) {
			even_full++;
		}
		if (steps % 2 == 1) {
			odd_full++;
		}
	}

	auto calculateP2 = [&]() {
		auto pow_n = num_gardens_from_start * num_gardens_from_start;
		auto pow_n1 = (num_gardens_from_start + 1) * (num_gardens_from_start + 1);

		return pow_n1 * odd_full + pow_n * even_full - (num_gardens_from_start + 1) * odd_corners + num_gardens_from_start * even_corners;
	};

	std::cout << "part 2: the number of possible ending steps is: " << calculateP2() << '\n';

	return 0;
}
