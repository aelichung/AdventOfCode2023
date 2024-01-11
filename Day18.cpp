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

	std::vector<Coord> vertices;

	std::ifstream ifs("input.txt");
	std::string s;
	Coord vertex{ 0,0 };	
	int64_t perimeter = 0;

	while (ifs.good()) {
		std::getline(ifs, s);
		char directionChar = s[0];
		auto endIndex = s.find(" (#");
		std::string multiString = s.substr(2, endIndex - 2);
		int64_t multiplier = std::stoi(multiString);
		std::cout << "direction: " << directionChar << " multiplier: " << multiplier << '\n';

		vertex = vertex +  move_direction(directionChar, multiplier);
		perimeter += multiplier;
		vertices.push_back(vertex);
	}
	ifs.clear();

	for (auto v : vertices) {
		std::cout << v.x << ", " << v.y << '\n';
	}

	auto area_shoelace = [&]() {
		int64_t area = 0;
		
		for (int i = 0; i < vertices.size(); i++) {			
			auto j = (i + 1) % vertices.size();
			area = area + vertices[i].x * vertices[j].y;
			area = area - vertices[i].y * vertices[j].x;
		}
		area /= 2;		
		return area;
	};

	auto inner_area = [&]() {
		return area_shoelace() - (perimeter / 2) + 1;
	};

	std::cout << "part 1: " << inner_area() + perimeter << '\n';

	return 0;
}