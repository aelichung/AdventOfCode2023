//rn = 1 becomes 30.
//cm - becomes 253.
//qp = 3 becomes 97.
//cm = 2 becomes 47.
//qp - becomes 14.
//pc = 4 becomes 180.
//ot = 9 becomes 9.
//ab = 5 becomes 197.
//pc - becomes 48.
//pc = 6 becomes 214.
//ot = 7 becomes 231.
//In this example, the sum of these results is 1320.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

long long HASH(std::string input) {
	long long currentValue = 0;
	for (auto c : input) {
		auto asciiValue = int(c);
		currentValue += asciiValue;
		currentValue *= 17ll;
		currentValue = currentValue % 256ll;
	}
	return currentValue;
}

int main() {
	std::ifstream ifs("input.txt");
	std::string s;
	long long findPosition = 0;
	std::string delimiter = ",";
	long long sum = 0;

	std::getline(ifs, s);
	ifs.close();
		
	auto delimiterPosition = s.find(delimiter);
	while (delimiterPosition != std::string::npos) {
		std::string token = s.substr(findPosition, delimiterPosition - findPosition);
		long long hash = HASH(token);
		sum += hash;
		findPosition = delimiterPosition + 1;
		delimiterPosition = s.find(delimiter, findPosition);
		std::cout << token <<": hash is "<< hash << '\n';
	}

	std::string token = s.substr(findPosition, s.length() - findPosition);
	long long hash = HASH(token);
	std::cout << token << ": hash is " << hash << '\n';
	sum += hash;
		
	std::cout << "sum: " << sum << '\n';
	return 0;
}
