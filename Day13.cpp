#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>

struct MirrorNotes {
public:
	std::vector<std::string> horizontalStrings;
	std::vector<std::string> verticalStrings;
	bool smudgeFound = false;

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

	bool OffByOne(std::string stringA, std::string stringB) {
		int count = 0;
		for (long long i = 0; i < stringA.length(); i++) {
			if (stringA[i] != stringB[i]) {
				++count;
				if (count > 1) return false;
			}
		}
		return count == 1;
	}

	std::vector<long long> FindReflectionLine(std::vector<std::string> stringsVector)
	{
		std::vector<long long> reflectionEdge;

		for (long long i = 1; i < stringsVector.size(); i++)
		{
			int smudgeCount = 0;
			bool shouldCheck = false;

			std::string stringA = stringsVector[i - 1];
			std::string stringB = stringsVector[i];
			if ( stringA == stringB) {
				shouldCheck = true;
			}
			else if (OffByOne(stringA, stringB)) {
				++smudgeCount;
				shouldCheck = true;
				if (smudgeCount > 1) 
				{
					shouldCheck = false;
					continue;
				}
			}

			if (shouldCheck) {
				//potential mirror edge, expand out until we meet an edge or if the edge doesn't match
				long long greaterIndex = i;
				long long lesserIndex = i - 1;
				long long maxEdge = stringsVector.size() - 1;
				long long minEdge = 0;
				auto nextStringA = stringsVector[lesserIndex];
				auto nextStringB = stringsVector[greaterIndex];
				bool isOffByOne = OffByOne(nextStringA, nextStringB);

				do {
					if (greaterIndex == maxEdge || lesserIndex == minEdge) {
						if (smudgeCount == 1) {
							reflectionEdge.push_back(i - 1);
							reflectionEdge.push_back(i);
							smudgeFound = true;
							return reflectionEdge;
						}
						shouldCheck = false;
						continue;
					}
					++greaterIndex;
					--lesserIndex;
					nextStringA = stringsVector[lesserIndex];
					nextStringB = stringsVector[greaterIndex];
					
					isOffByOne = OffByOne(nextStringA, nextStringB);
					if (isOffByOne) {
						++smudgeCount;
						if (smudgeCount > 1)
						{
							shouldCheck = false;
							break;
						}
					}
				} while ( (nextStringA == nextStringB || isOffByOne) && smudgeCount <= 1 && shouldCheck);
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
		auto above = note.NumberOfRowsAbove();
		auto left = note.NumberOfColumnsToTheLeft();
		std::cout << "note number of columns to the left: " << left << "\n";
		std::cout << "note number of rows above: " << above << "\n";
		verticalSum += left;
		horizontalSum += above;
	}

	long long summary = verticalSum + 100 * horizontalSum;
	std::cout << "notes summary: " << summary << '\n';
	
	return 0;
}