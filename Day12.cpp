#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <regex>

long ProcessWorkingSpring(long index, std::string path, long currentConditionIndex, std::vector<long> conditions);
long ProcessBrokenSpring(long index, std::string path, long currentConditionIndex, std::vector<long> conditions);
long ProcessUnknown(long index, std::string path, long currentConditionIndex, std::vector<long> conditions);
long ProcessNext(long index, std::string path, long currentConditionIndex, std::vector<long> conditions);



long GetConditionSpace(long currentConditionIndex, std::vector<long> conditions) {
    long sum = 0;
    if (currentConditionIndex >= conditions.size()) return 0l;

    for (long i = currentConditionIndex; i < conditions.size(); i++) {
        sum += conditions[i];
    }
    //add the commas as a 1
    sum += conditions.size() - 1 - currentConditionIndex;
    return sum;
}

long ProcessWorkingSpring(long index, std::string path, long currentConditionIndex, std::vector<long> conditions) {
    auto remainingPathSpace = path.length() - index - 1;
    auto remainingConditionSpace = GetConditionSpace(currentConditionIndex, conditions);

    if (remainingConditionSpace > remainingPathSpace) return 0l;

    auto remainderString = path.substr(index);
    bool remainderContainsBrokenSpring = remainderString.find('#') != std::string::npos;

    if (!remainderContainsBrokenSpring && remainingConditionSpace == 0) return 1l;

    long currentIndex = index + 1;
    return ProcessNext(currentIndex, path, currentConditionIndex, conditions);
}

long ProcessBrokenSpring(long index, std::string path, long currentConditionIndex, std::vector<long> conditions) {
    if (currentConditionIndex >= conditions.size()) return 0l;
    auto condition = conditions[currentConditionIndex];
    auto pathLength = path.length();
    auto IndexAfterRun = condition + index;
    // our path is not big enough to accomodate the run that we need
    if (IndexAfterRun - 1 > pathLength) return 0l;
    auto portion = path.substr(index, condition);       
    // found a working spring in our run,  we can't fill the condition
    if (portion.find('.') != std::string::npos) 
        return 0l;
       
    // our path ends exactly as the run completes and we have fulfilled the last condition
    if (IndexAfterRun == pathLength && currentConditionIndex == conditions.size()-1) return 1l;
   
    // the spring at the end of our run is broken, we can't fill the condition
    if (path[IndexAfterRun] == '#') return 0l; 

    // our path ends one character after the run completes and we have fulfilled the last condition
    auto currentIndex = index + condition + 1;
    if (currentIndex == pathLength && currentConditionIndex == conditions.size() - 1) return 1l;

    ++currentConditionIndex;    
    return ProcessNext(currentIndex, path, currentConditionIndex, conditions);
}

long ProcessUnknown(long index, std::string path, long currentConditionIndex, std::vector<long> conditions) {
    auto brokenBranch = ProcessBrokenSpring(index, path, currentConditionIndex, conditions);
    auto workingBranch = ProcessWorkingSpring(index, path, currentConditionIndex, conditions);
    return  brokenBranch + workingBranch;
}

long ProcessNext(long index, std::string path, long currentConditionIndex, std::vector<long> conditions) {
    //ran out of characters
    if (index == path.length()) return 0l;
    char c = path.at(index);
    switch (c) {
    case '?':
        return ProcessUnknown(index, path, currentConditionIndex, conditions);        
    case '.':
        return ProcessWorkingSpring(index, path, currentConditionIndex, conditions);        
    case '#':
        return ProcessBrokenSpring(index, path, currentConditionIndex, conditions);        
    default:
        return 0l;
    }
}

int main()
{
    std::ifstream ifs("input.txt");
    std::string s;

    std::regex springsChunkPattern(R"([\?.#]+(?=\s))");
    std::regex legendNumberPattern(R"((\d+))");

    long totalArrangementSum = 0l;

    while (ifs.good()) {
        std::getline(ifs, s);
        
        std::smatch springsChunkMatch;
        std::regex_search(s, springsChunkMatch, springsChunkPattern);
        std::string springsChunk = springsChunkMatch.str();

        std::sregex_token_iterator conditionIter(s.begin(), s.end(), legendNumberPattern, 1);
        std::sregex_token_iterator endIter;

        std::vector<long> conditions;
        while (conditionIter != endIter) {
            auto condition = std::stol(*conditionIter);
            conditions.push_back(condition);
            conditionIter++;
        }

        //auto expandedSpringsInput = springsChunk + 

        auto sum = ProcessNext(0l, springsChunk, 0l, conditions);
        std::cout << springsChunk << " " << sum << '\n';
        totalArrangementSum += sum;
    }

    ifs.clear();

    std::cout << "total arrangement sum: " << totalArrangementSum<< '\n';

    return 0;
}