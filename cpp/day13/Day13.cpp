#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<regex>

struct MirrorNotes {
public:
	std::vector<std::string> horizontalStrings;
	std::vector<std::string> verticalStrings;

	MirrorNotes(std::vector<std::string> hStrings) : horizontalStrings(hStrings)
	{
		for (long long i = 0; i < horizontalStrings.size(); i++) {
			std::string vString;
			for (auto& s : horizontalStrings) {
				vString += s[i];
			}
			verticalStrings.push_back(vString);
		}
	}
};

int main() {
	std::ifstream ifs("input.txt");
	std::string inputString;
	std::regex spacePattern{ R"(\s)" };

	//read the galaxy from input
	while (ifs.good())
	{		
		std::getline(ifs, inputString);
		
		std::smatch smatch;
		bool isSpace = std::regex_match(inputString, smatch, spacePattern);
		if (isSpace) {

		}
		std::vector<std::string> horizontalStrings;


		horizontalStrings.push_back(inputString);
	}
	ifs.close();	



	/*std::cout << "horizontal strings vector size: " << horizontalStrings.size() << '\n';
	std::cout << "vertical strings vector size: " << verticalStrings.size() << '\n';*/
	
	return 0;
}