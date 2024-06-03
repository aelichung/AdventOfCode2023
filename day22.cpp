#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
#include <regex>
#include <iostream>

struct position {
	int x, y, z;
};

struct brick {
	position position_a;
	position position_b;

	brick(position a, position b) : position_a(a), position_b(b) {
		
	}

	int get_min_z() const {
		return std::min(position_a.z, position_b.z);
	}

	int get_z_height() const{
		return abs(position_a.z - position_b.z) + 1;
	}

	int get_x_width() const {
		return abs(position_a.x - position_b.x) + 1;
	}

	int get_y_length() const {
		return abs(position_a.y - position_b.y) + 1;
	}

	void updateTransform(int z) {
		int z_a = position_a.z;
		int z_b = position_b.z;
		if ( z_a < z_b) {
			int offset = z_b - z_a;
			position_a.z = z;
			position_b.z = z + offset;
		}
		else if (z_b < z_a) {
			int offset = z_a - z_b;
			position_b.z = z;
			position_a.z = z + offset;
		}
		else {
			position_b.z = z;
			position_a.z = z;
		}		
	}
};

struct brick_compare_min {
	bool operator()(const brick& a, const brick& b) const {
		return a.get_min_z() > b.get_min_z();
	}
};

struct grid_plane {
	std::vector<std::string> grid;
	grid_plane(int num_rows, int num_cols) {
		int rows = num_rows;
		while (rows > 0) {
			std::string s(num_cols, '.');
			grid.push_back(s);
			rows--;
		}
	}

	void fillCells(int x1, int y1, int x2, int y2) {
		int min_x = std::min(x1, x2);
		int max_x = std::max(x1, x2);
		int min_y = std::min(y1, y2);
		int max_y = std::max(y1, y2);

		if (x1 == x2 && y1 == y2) {
			grid[y1][x1] = '#';
			return;
		}

		if (x1 == x2) {			
			while (min_y <= max_y) {
				grid[min_y][x1] = '#';
				min_y++;
			}
			return;
		}

		if (y1 == y2) {
			while (min_x <= max_x) {
				grid[y1][min_x] = '#';
				min_x++;
			}
			return;
		}

		while (min_y <= max_y) {
			int x = min_x;
			while (x <= max_x) {
				grid[min_y][x] = '#';				
				x++;
			}
			min_y++;
		}
	}

	void display() {
		for (int i = 0; i < grid.size(); i++) {
			std::cout << grid[i] << '\n';
		}		
	}
	
	bool hasCollision()const {

	}
};

int main() {

	//read input and create bricks
	std::ifstream ifs("input-test.txt");
	std::string input;
	std::regex pattern{ R"(([0-9]+),([0-9]+),([0-9])+\~([0-9]+),([0-9]+),([0-9]+))" };
	std::smatch match;

	std::priority_queue<brick, std::vector<brick>, brick_compare_min> bricks_pq_min;
	int min_x = 0, max_x = 0, min_y = 0, max_y = 0;

	auto set_min_max = [&min_x, &max_x, &min_y, &max_y](int x, int y) {
		if (min_x > x) {
			min_x = x;
		}
		if (max_x < x) {
			max_x = x;
		}
		if (min_y > y) {
			min_y = y;
		}
		if (max_y < y) {
			max_y = y;
		}
	};

	while (ifs.good()) {
		std::getline(ifs, input);
		if (std::regex_search(input, match, pattern)) {
			int x = std::stoi(match[1].str());
			int y = std::stoi(match[2].str());
			int z = std::stoi(match[3].str());
			position p_a{ x,y,z};

			set_min_max(x, y);

			x = std::stoi(match[4].str());
			y = std::stoi(match[5].str());
			z = std::stoi(match[6].str());

			set_min_max(x, y);

			position p_b{ x,y,z };
			brick b{ p_a,p_b };
			bricks_pq_min.push(b); //place brick in priority queue sorting smallest z in ascending order
		}				
	}

	std::cout << "number of bricks: " << bricks_pq_min.size() << '\n';
	std::cout << "min x: " << min_x << ", max x: "<< max_x << ", min y: "<< min_y<<", max y: " << max_y << '\n';

	std::vector<grid_plane> point_cloud;
	int rows = max_y - min_y + 1;
	int cols = max_x - min_x + 1;
	grid_plane grid{ rows,cols };
	
	//process brick starting with lowest z
	while (!bricks_pq_min.empty()) {
		brick top = bricks_pq_min.top();
		int min_z = top.get_min_z();
		int x1 = top.position_a.x, y1 = top.position_a.y, x2 = top.position_b.x, y2 = top.position_b.y;
		int size = point_cloud.size();
	
		//descend the brick until it collides
		int i = size - 1;
		
		while (i >=0) {
			grid_plane next = point_cloud[i];
			if (next.hasCollision()) break;
			--i;
		}
		int new_z = i + 2;
		top.updateTransform(new_z);
		
		//check has collision for collision, 
		//store the brick in a unordered dictionary where the x,y,z position is the key
		//find the brick responsible for the collision with this brick
		//the current brick is structurally "dependent" on the brick it collided with
		
		
		//check if the collision landing is at the top
		//todo
		int height = top.get_z_height();
		min_z = top.get_min_z();
		while (height > 0) {
			grid_plane g = grid;
			g.fillCells(x1, y1, x2, y2);
			point_cloud.push_back(g);
			g.display();
			height--;
		}

		bricks_pq_min.pop();
	}
		

		
	

	//process brick steps
	//1 if there are no grid planes in the vector, 

	//if brick has a position with a z of 1, then we create a grid plane
	//a grid plane is a 2d array of size maximum x - minimum x and maximum y - minimum y
	//we can represent this gridplane as an array of strings where a '.' represents empty space and '#' represents a space occupied by a cube

	//after creating the gridplane, we process the next brick in the queue. 
	//we check to see if the brick shares any z coordinates with the current grid plane (if z == 1 and we just made grid plane for z == 1)
	//if so, we update the grid plane and process the next brick

	//if not, we check to see if the brick shares any x,y coordinates with the current grid plane
	//if there is, then a collision is registered and we create a new grid plane 

	return 0;
}