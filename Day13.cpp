#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>

struct MirrorNotes {
public:
	std::vector<std::string> horizontalStrings;
	std::vector<std::string> verticalStrings;

	MirrorNotes(std::vector<std::string> hStrings) : horizontalStrings(hStrings)
	{
		for (long long charIndex = 0; charIndex < horizontalStrings.at(0).length(); charIndex++) {
			std::string vString;
			for (long long rowIndex = 0; rowIndex < horizontalStrings.size(); rowIndex++) {
				char c = horizontalStrings.at(rowIndex).at(charIndex);
				vString += c;
			}
			verticalStrings.push_back(vString);
		}
	}

	std::vector<long long> FindReflectionLine(std::vector<std::string> stringsVector)
	{
		std::vector<long long> reflectionEdge;
		for (long long i = 1; i < stringsVector.size(); i++)
		{
			if (stringsVector[i] == stringsVector[i - 1]) {
				//potential mirror edge, expand out until we meet an edge or if the edge doesn't match
				long long greaterIndex = i;
				long long lesserIndex = i - 1;
				long long maxEdge = stringsVector.size() - 1;
				long long minEdge = 0;

				do {
					if (greaterIndex == maxEdge || lesserIndex == minEdge) {
						reflectionEdge.push_back(i - 1);
						reflectionEdge.push_back(i);
						return reflectionEdge;
					}
					++greaterIndex;
					--lesserIndex;
				} while (stringsVector[greaterIndex] == stringsVector[lesserIndex]);
			}
		}
		return reflectionEdge;
	}

	long long NumberOfColumnsToTheLeft() {
		std::vector<long long> verticalEdges = FindReflectionLine(verticalStrings);
		if (verticalEdges.size() == 0) return 0;
		long long leftEdge = verticalEdges[0];
		return leftEdge + 1;
	}

	long long NumberOfRowsAbove() {
		std::vector<long long> horizontalEdges = FindReflectionLine(horizontalStrings);
		if (horizontalEdges.size() == 0) return 0;
		long long upperEdge = horizontalEdges[0];
		return upperEdge + 1;
	}
};

int main() {
	std::ifstream ifs("input.txt");
	std::string inputString;

	std::vector<std::string> horizontalStrings;
	std::vector<MirrorNotes> notes;

	//read the galaxy from input
	while (ifs.good())
	{		
		std::getline(ifs, inputString);

		if (inputString.empty()) {
			MirrorNotes note = { horizontalStrings };
			notes.push_back(note);
			horizontalStrings.clear();
			continue;
		}		

		horizontalStrings.push_back(inputString);
	}
	ifs.close();	
	MirrorNotes note = { horizontalStrings };
	notes.push_back(note);

	std::cout << "notes size: " << notes.size() << '\n';

	long long verticalSum = 0;
	long long horizontalSum = 0;
	for (auto& note : notes) {
		verticalSum += note.NumberOfColumnsToTheLeft();
		horizontalSum += note.NumberOfRowsAbove();
	}

	long long summary = verticalSum + 100 * horizontalSum;
	std::cout << "notes summary: " << summary << '\n';
	
	return 0;
}