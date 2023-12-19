#include<iostream>
#include<sstream>
#include<fstream>


int main() {
	std::ifstream ifs("input.txt");
	std::string inputString;

	//read the galaxy from input
	while (ifs.good())
	{		
		std::getline(ifs, inputString);

	}
	ifs.close();	
	
	return 0;
}