//referencing a lot of code from https://medium.com/@simontoth/daily-bit-e-of-c-advent-of-code-day-17-12a1c452968b for today's problem
//going to heavily comment code patterns that are unfamiliar to me, hopefully absorb some good practices! :D

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string_view>
#include <ranges>
#include <queue>
#include  <array>
#include <cstdint>

namespace { //not sure how this unnamed namespace is different from a global namespace??
	struct Coord {
		int64_t row, col; //int 64 is not guaranteed to exist? int_64_t will always use 64 bits, with 2's complement and no padding
		auto operator <=>(const Coord&) const = default; //overloading the spaceship operator to take care of less than, greater than and equals operator overloads. default sets this up with the default <=> function but not sure how this works with this struct's members...
		Coord operator+(const Coord& other) const { //not sure what the extra const at the end is doing here
			Coord result = *this; 
			result.row += other.row;
			result.col += other.col;
			return result;
		}
		Coord operator*(int multiplier) const {
			Coord result = *this;
			result.row *= multiplier;
			result.col *= multiplier;
			return result;
		}
	};

	struct State {
		Coord position, direction;
		int64_t steps, heat_loss;
		bool operator<(const State& other) const {
			return heat_loss > other.heat_loss;
		}
	};
}

template <> struct std::hash<Coord> {
	std::size_t operator()(const Coord& c) const noexcept { //no except performs a compile time check toensure that expression will not throw exceptions, I see this used a lot with operator overloads
		std::size_t h1 = std::hash<int64_t>{}(c.row);
		std::size_t h2 = std::hash<int64_t>{}(c.row); //not sure why we are hashing the row again, is this a typo??
		return h1 ^ (h2 << 1);
	}
};

int main() {
	std::vector<std::string> grid;

	std::ifstream ifs("input.txt");
	std::string s;	

	while (ifs.good()) {
		std::getline(ifs, s);
		grid.push_back(s);
	}
	ifs.clear();

	//get heat loss
	auto loss_at = [&](Coord position) {
		return grid[position.row][position.col] - '0';
	};

	//check boundaries
	auto can_step = [&](Coord coord) {
		if (coord.row < 0 || coord.col < 0)
			return false;
		if (coord.row >= std::ssize(grid)) //std::size & ssize allows you to avoid unwanted unsigned-ness 
			return false;
		if (coord.col >= std::ssize(grid[0]))
			return false;
		return true;
	};

	//returns direction to the left
	auto left_direction = [](Coord direction) {
		if (direction.row != 0)
			return Coord{ 0, direction.row };
		return Coord{ -direction.col, 0 };
	};

	//returns direction to the right
	auto right_direction = [](Coord direction) {
		if (direction.row != 0)
			return Coord{ 0, -direction.row };
		return Coord{ direction.col, 0 };
	};

	constexpr auto max_steps = 10;
	using Best = std::array<std::array<int64_t, max_steps>, 4>;
	std::vector<std::vector<Best>> best(grid.size(), std::vector<Best>(grid[0].size(), 
		[] {
				std::array<std::array<int64_t, max_steps>, 4> result;
				for (auto& direction : result) {
					for (auto& distance : direction) {
						distance = INT64_MAX;
					}
				}
				return result;
	}())); //wow this 4d array initialization is really hard to read 

	//returns index for given direction
	auto dir_to_int = [&](Coord direction) {
		if (direction.row == -1)
			return 0;
		if (direction.row == 1)
			return 1;
		if (direction.col == -1)
			return 2;
		return 3;
	};

	std::priority_queue<State> q;
	Coord destination = { std::ssize(grid) -1, std::ssize(grid[0]) -1 };

	//adds a given state to the queue if it is better (less heat loss)
	//than our best known for that position, direction, and remaining steps

	auto push_if_better = [&](State s) {
		if (s.steps == 0) {
			best[s.position.row][s.position.col][dir_to_int(s.direction)][0] = s.heat_loss;
			q.push(s);
			return;
		}
		if (best[s.position.row][s.position.col][dir_to_int(s.direction)][s.steps - 1] > s.heat_loss) {
			best[s.position.row][s.position.col][dir_to_int(s.direction)][s.steps - 1] = s.heat_loss;
			q.push(s);
		}
	};

	//two starting states, from {0,0}, one going right, and one going down
	//steps and heat loss are both 0 initially
	push_if_better({ {0,0}, {1, 0},  0, 0 });
	push_if_better({ {0,0}, {0,1}, 0,0 });

	while (not q.empty()) { //i like the "not" keyword usage here
		auto [pos, dir, steps, heat_loss] = q.top(); //cool way to define all the variables in a big array for local usage, reminds me of tuples
		//if we are at our destination, report the heat loss. because we are using a priority queue this is a guanrantee of the min value
		if (pos == destination)
		{
			std::cout << "part 2:" << heat_loss;
			return 0;
		}
		q.pop();

		//can only go forward 10 steps in p2
		if (steps < max_steps && can_step(pos + dir)) {
			push_if_better(
				{ pos + dir, dir, steps + 1, heat_loss + loss_at(pos + dir) }
			);
		}

		//can only turn after 4 steps, also need to make sure not to run into wall after turning
		if (steps >= 4) {
			if (can_step(pos + (left_direction(dir) * 4))) {
				push_if_better({ 
					pos + left_direction(dir), 
					left_direction(dir),
					1, 
					heat_loss + loss_at(pos + left_direction(dir))
					});
			}

			if (can_step(pos + right_direction(dir)* 4)) {
				push_if_better({ 
					pos + right_direction(dir), 
					right_direction(dir), 
					1, 
					heat_loss + loss_at(pos + right_direction(dir)) });
			}
		}

	}

	return 0;
}