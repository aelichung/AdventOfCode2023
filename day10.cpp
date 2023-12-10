#include<iostream>
#include<sstream>
#include<fstream>
#include<regex>
#include<numeric>

int main() {
	std::ifstream ifs("input.txt");
	std::string inputString;

	while (ifs.good()) {
		std::getline(ifs, inputString);

	}
	ifs.close();

	std::cout << "part 1 " <<  std::endl;
	std::cout << "part 2 " <<  std::endl;

	return 0;
}