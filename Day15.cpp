#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <regex>

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

void HASHMAP(std::map<int, std::vector<std::string>>& boxes, std::string label, char operation, std::string focalLengthString = "") {
	//get box number
	long long boxNum = HASH(label);
	std::vector<std::string>& box = boxes[boxNum];
	long long lensPosition = -1;

	switch (operation) {
	case '-':	
		for (long long i = 0; i < box.size(); i++) {
			auto s = box[i];
			std::string boxLabel = s.substr(0, s.find(" "));
			if (boxLabel == label) {
				lensPosition = i;
				break;
			}
		}
		if (lensPosition != -1) {
			box.erase(box.begin() + lensPosition);
		}
		break;
	case '=':		
		for (long long i = 0; i < box.size(); i++) {
			auto s = box[i];
			std::string boxLabel = s.substr(0, s.find(" "));
			if (boxLabel == label) {
				lensPosition = i;
				break;
			}
		}
		std::string newLabel = label + " " + focalLengthString;
		if (lensPosition != -1) {
			box[lensPosition] = newLabel;
		}
		else {
			box.push_back(newLabel);
		}
		break;
	}
	//boxes[boxNum] = box;
}

long long GetFocusingPower(long long boxNumber, long long lensSlotNumber, long long focalLength) {
	return (boxNumber + 1) * lensSlotNumber * focalLength;
}

int main() {
	std::ifstream ifs("input.txt");
	std::string s;
	long long findPosition = 0;
	std::string delimiter = ",";
	long long sum = 0;

	std::getline(ifs, s);
	ifs.close();
		
	//part1
	//auto delimiterPosition = s.find(delimiter);
	//while (delimiterPosition != std::string::npos) {
	//	std::string token = s.substr(findPosition, delimiterPosition - findPosition);
	//	long long hash = HASH(token);
	//	sum += hash;
	//	findPosition = delimiterPosition + 1;
	//	delimiterPosition = s.find(delimiter, findPosition);
	//	std::cout << token <<": hash is "<< hash << '\n';
	//}

	//std::string token = s.substr(findPosition, s.length() - findPosition);
	//long long hash = HASH(token);
	//std::cout << token << ": hash is " << hash << '\n';
	//sum += hash;

	std::map<int, std::vector<std::string>> boxes;
	int counter = 0;
	while (counter < 256) {
		auto vec = std::vector<std::string>();
		boxes[counter] = vec;
		++counter;
	}

	std::regex boxPattern(R"(([a-z]+)([=-])(\d*),?)");
	std::sregex_iterator begin = std::sregex_iterator(s.begin(), s.end(), boxPattern);
	auto end = std::sregex_iterator();
	
	for (std::sregex_iterator i = begin; i != end; i++) {
		std::smatch match = *i;
		std::string label = match[1];
		std::string operation = match[2];
		std::string focalLength = match[3];
		//std::cout << "label: " << label << " , operation: " << operation << ", focal length: " << focalLength << '\n';
		HASHMAP(boxes, label, operation[0], focalLength);
	}

	for (long long i = 0; i < boxes.size(); i++) {
		std::string boxContents;
		for (auto s : boxes[i]) {
			boxContents += s + " ";
		}
		std::cout << " box " << i << ": " << boxContents << '\n';
		std::vector<std::string> box = boxes[i];

		if (box.empty()) {
			continue;
		}
		for (long long lensIndex = 1; lensIndex <= box.size(); lensIndex++) {			
			std::string labelString = box[lensIndex - 1];
			char c = labelString[labelString.length() - 1];
			std::string focalLengthString{c};
			long long focalLength = std::stoll(focalLengthString);
			sum += GetFocusingPower(i, lensIndex, focalLength);
		}
	}
	
	std::cout << "sum: " << sum << '\n';
	return 0;
}
