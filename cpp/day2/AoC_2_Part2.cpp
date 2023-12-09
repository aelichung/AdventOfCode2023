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

std::vector<std::string> colorStrings= {
	" red",
	" green",
	" blue"
};

std::map<std::string, int> colorStringsMaxPairs = {
	{" red", RED_MAX},
	{" green", GREEN_MAX},
	{" blue", BLUE_MAX} };

bool isGamePossible(std::string s) {
	if (s == "") return false;
	for (auto& colorString : colorStrings) {
		for (int i = 0; i < s.length(); i++) {
			std::string numberSubString;
			std::string colorSubString = s.substr(i, colorString.length());
			if (colorSubString == colorString) {
				//find the digits before the color substring
				for (int j = i - 1; std::isdigit(s[j]); j--) {
					std::string insert{ s[j] };
					numberSubString.insert(0, insert);
				}
				const int numColor{ std::stoi(numberSubString) };
				if (numColor > colorStringsMaxPairs[colorString]) {
					return false;
				}
			}
		}
	}
	return true;
}

int gamePower(std::string s) {
	if (s == "") return 0;
	int power = 1;
	for (auto& colorString : colorStrings) {
		int colorMax = 0;
		for (int i = 0; i < s.length(); i++) {
			std::string numberSubString;
			std::string colorSubString = s.substr(i, colorString.length());
			if (colorSubString == colorString) {
				//find the digits before the color substring
				for (int j = i - 1; std::isdigit(s[j]); j--) {
					std::string insert{ s[j] };
					numberSubString.insert(0, insert);
				}
				const int numColor{ std::stoi(numberSubString) };
				if (numColor > colorMax) {
					colorMax = numColor;
				}
			}
		}
		power *= colorMax;
	}
	return power;
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
		int power = gamePower(s);
		std::cout << "Game power for game number" << lineCounter << " is " << power<< '\n';
		lineCounter++;
		sum += power;
	}

	std::cout << "sum: " << sum;
	return 0;
}

