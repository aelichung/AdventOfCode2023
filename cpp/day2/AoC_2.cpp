#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

const int RED_MAX = 12;
const int GREEN_MAX = 13;
const int BLUE_MAX = 14;

std::map<std::string, std::vector<int>> colorStringsCountPairs = {
	{" red", std::vector<int>()},
	{" green", std::vector<int>()},
	{" blue", std::vector<int>()} };

std::map<std::string, int> colorStringsMaxPairs = {
	{" red", RED_MAX},
	{" green", GREEN_MAX},
	{" blue", BLUE_MAX} };

bool isGamePossible(std::string s) {
	if (s == "") return false;
	for (auto& pair : colorStringsCountPairs) {
		for (int i = 0; i < s.length(); i++) {
			std::string numberSubString;
			std::string colorSubString = s.substr(i, pair.first.length());
			if (colorSubString == pair.first) {
				//find the digits before the color substring
				for (int j = i - 1; std::isdigit(s[j]); j--) {
					std::string insert{ s[j] };
					numberSubString.insert(0, insert);
				}
				const int numColor{ std::stoi(numberSubString) };
				if (numColor > colorStringsMaxPairs[pair.first]) {
					return false;
				}
			}
		}
	}
	return true;
}

int main() {

	std::ifstream ifs("input_day2.txt", std::ifstream::in);

	std::string s;
	int sum = 0;
	int lineCounter = 1;
	
	while (ifs.good()) {
		std::getline(ifs, s);					

		//compare the red, green, and blue values to their max
		//early out if the actual values are greater than the max	
		if(isGamePossible(s))
		{
			sum += lineCounter;
			std::cout << "Game number " << lineCounter << " is possible" << '\n';
		}
		lineCounter++;
	}

	std::cout << "sum: " << sum;
	return 0;
}

