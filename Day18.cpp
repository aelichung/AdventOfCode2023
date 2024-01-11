#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

namespace {
	struct Coord {
		int64_t x, y;
		Coord operator+(const Coord& other) {
			Coord result = *this;
			result.x += other.x;
			result.y += other.y;
			return result;
		}
	};
}

int main() {

	auto move_direction = [](char c, int64_t multiplier) {
		Coord result;
		switch (c) {
		case 'R':
			result = { multiplier, 0 };
			break;
		case 'D':
			result = { 0, multiplier };
			break;
		case 'L':
			result = { -multiplier, 0 };
			break;
		case 'U':
			result = { 0, -multiplier };
			break;
		}
		return result;
	};

	std::vector<Coord> vertices_part1;
	std::vector<Coord> vertices_part2;

	std::ifstream ifs("input.txt");
	std::string s;
	Coord vertex1{ 0,0 };	
	Coord vertex2{ 0,0 };
	
	int64_t perimeter1 = 0;
	int64_t perimeter2 = 0;

	auto numToChar = [](char num) {
		char result;
		switch (num) {
		case '0':
			result = 'R';
			break;
		case '1':
			result = 'D';
			break;
		case '2':
			result = 'L';
			break;
		case '3':
			result = 'U';
			break;
		}
		return result;
	};

	while (ifs.good()) {
		std::getline(ifs, s);
		char directionChar = s[0];
		auto endIndex = s.find(" (#");
		std::string multiString = s.substr(2, endIndex - 2);
		int64_t multiplier = std::stoi(multiString);
		std::cout << "direction: " << directionChar << " multiplier: " << multiplier << '\n';

		vertex1 = vertex1 +  move_direction(directionChar, multiplier);
		perimeter1 += multiplier;
		vertices_part1.push_back(vertex1);

		directionChar = numToChar(s[s.size() - 2]);
		auto startIndex = s.find("#");
		multiString = s.substr(startIndex + 1, 5);
		multiplier = std::stoul(multiString, nullptr, 16);
		vertex2 = vertex2 + move_direction(directionChar, multiplier);
		perimeter2 += multiplier;
		vertices_part2.push_back(vertex2);
		std::cout << "direction: " << directionChar << " multiplier: " << multiplier << '\n';
	}
	ifs.clear();

	for (auto v : vertices_part1) {
		std::cout << v.x << ", " << v.y << '\n';
	}

	auto area_shoelace = [&](const std::vector<Coord>& vertices) {
		int64_t area = 0;
		
		for (int i = 0; i < vertices.size(); i++) {
			auto j = (i + 1) % vertices.size();
			area = area + vertices[i].x * vertices[j].y;
			area = area - vertices[i].y * vertices[j].x;
		}
		area /= 2;		
		return area;
	};

	auto area = [&](const std::vector<Coord>& vertices, const int64_t& perimeter) {
		return area_shoelace(vertices) + (perimeter / 2) + 1;
	};

	std::cout << "part 1: " << area(vertices_part1, perimeter1) << '\n';
	std::cout << "part 2: " << area(vertices_part2, perimeter2)<< '\n';

	return 0;
}