#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <regex>

long long ProcessWorkingSpring(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions);
long long ProcessBrokenSpring(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions);
long long ProcessUnknown(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions);
long long ProcessNext(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions);

//this function is at position 4 for the outermost vector
long long GetConditionSpace(std::vector<std::vector<std::vector<long long>>>& table, long long currentConditionIndex, std::vector<long long> conditions) {
    auto result = table.at(4).at(0).at(currentConditionIndex);
    if (result != -1) {
        return result;
    }
    long long sum = 0;
    if (currentConditionIndex >= conditions.size())
    {        
        table.at(4).at(0).at(currentConditionIndex) = 0ll;;
        return 0ll;
    }

    for (long long i = currentConditionIndex; i < conditions.size(); i++) {
        sum += conditions[i];
    }
    //add the commas as a 1
    sum += conditions.size() - 1 - currentConditionIndex;    
    table.at(4).at(0).at(currentConditionIndex) = sum;
    return sum;
}

//this function is at position 3 for the outermost vector
long long ProcessWorkingSpring(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions) 
{
    auto result = table.at(3).at(index).at(currentConditionIndex);
    if (result != -1) {
        return result;
    }
    auto remainingPathSpace = path.length() - index - 1;
    auto remainingConditionSpace = GetConditionSpace(table,currentConditionIndex, conditions);

    if (remainingConditionSpace > remainingPathSpace)
    {
        long long emptyResult = 0l;
        table.at(3).at(index).at(currentConditionIndex) = emptyResult;
        return emptyResult;
    }
    auto remainderString = path.substr(index);
    bool remainderContainsBrokenSpring = remainderString.find('#') != std::string::npos;

    if (!remainderContainsBrokenSpring && remainingConditionSpace == 0)
    {        
        table.at(3).at(index).at(currentConditionIndex) = 1ll;
        return 1ll;
    }
    long long currentIndex = index + 1;
    return ProcessNext(table,currentIndex, path, currentConditionIndex, conditions);
}

//this function is at position 2 for the outermost vector
long long ProcessBrokenSpring(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions) {
    auto result = table.at(2).at(index).at(currentConditionIndex);
    if (result != -1) {
        return result;
    }
    if (currentConditionIndex >= conditions.size()) 
    {
        table.at(2).at(index).at(currentConditionIndex) = 0ll;
        return 0ll;
    }
    auto condition = conditions[currentConditionIndex];
    auto pathLength = path.length();
    auto IndexAfterRun = condition + index;
    // our path is not big enough to accomodate the run that we need
    if (IndexAfterRun - 1 > pathLength)
    {
        table.at(2).at(index).at(currentConditionIndex) = 0ll;
        return 0ll;
    }
    auto portion = path.substr(index, condition);       
    // found a working spring in our run,  we can't fill the condition
    if (portion.find('.') != std::string::npos) 
    {        
        table.at(2).at(index).at(currentConditionIndex) = 0ll;
        return 0ll;
    }
          
    // our path ends exactly as the run completes and we have fulfilled the last condition
    if (IndexAfterRun == pathLength && currentConditionIndex == conditions.size()-1)
    {        
        table.at(2).at(index).at(currentConditionIndex) = 1ll;
        return 1ll;
    }
   
    // the spring at the end of our run is broken, we can't fill the condition
    if (path[IndexAfterRun] == '#')
    {        
        table.at(2).at(index).at(currentConditionIndex) = 0ll;
        return 0ll;
    }

    // our path ends one character after the run completes and we have fulfilled the last condition
    auto currentIndex = index + condition + 1;
    if (currentIndex == pathLength && currentConditionIndex == conditions.size() - 1)
    {        
        table.at(2).at(index).at(currentConditionIndex) = 1ll;
        return 1ll;
    }

    ++currentConditionIndex;    
    return ProcessNext(table,currentIndex, path, currentConditionIndex, conditions);
}

//this function is at position 1 for the outermost vector
long long ProcessUnknown(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions) {
    auto result = table.at(1).at(index).at(currentConditionIndex);
    if (result != -1) {
        return result;
    }
    auto brokenBranch = ProcessBrokenSpring(table, index, path, currentConditionIndex, conditions);
    auto workingBranch = ProcessWorkingSpring(table, index, path, currentConditionIndex, conditions);
    table.at(1).at(index).at(currentConditionIndex) = brokenBranch + workingBranch;
    return  brokenBranch + workingBranch;
}

//this function is at position 0 for the outermost vector
long long ProcessNext(std::vector<std::vector<std::vector<long long>>>& table, long long index, std::string path, long long currentConditionIndex, std::vector<long long> conditions) {
    //check table
    long long result = table.at(0).at(index).at(currentConditionIndex);
    if ( result!= -1) {
        return result;
    }
    //ran out of characters
    if (index == path.length())
    {        
        table.at(0).at(index).at(currentConditionIndex) = 0ll;
        return 0ll;
    }
        
    char c = path.at(index);
    switch (c) {
    case '?':
        return ProcessUnknown(table,index, path, currentConditionIndex, conditions);        
    case '.':
        return ProcessWorkingSpring(table, index, path, currentConditionIndex, conditions);
    case '#':
        return ProcessBrokenSpring(table, index, path, currentConditionIndex, conditions);        
    default:
        return 0ll;
    }
}

int main()
{
    std::ifstream ifs("input.txt");
    std::string s;

    std::regex springsChunkPattern(R"([\?.#]+(?=\s))");
    std::regex legendNumberPattern(R"((\d+))");

    long long totalArrangementSum = 0ll;

    while (ifs.good()) {
        std::getline(ifs, s);
        
        std::smatch springsChunkMatch;
        std::regex_search(s, springsChunkMatch, springsChunkPattern);
        std::string springsChunk = springsChunkMatch.str();

        std::sregex_token_iterator conditionIter(s.begin(), s.end(), legendNumberPattern, 1);
        std::sregex_token_iterator endIter;

        std::vector<long long> conditions;
        while (conditionIter != endIter) {
            auto condition = std::stoll(*conditionIter);
            conditions.push_back(condition);
            conditionIter++;
        }

        auto expandedSpringsInput = springsChunk + "?" + springsChunk + "?" + springsChunk + "?" + springsChunk + "?" + springsChunk;
        std::vector<long long> expandedConditions;

        for (long long i = 0; i < 5; i++) {
            expandedConditions.insert(expandedConditions.end(), conditions.begin(), conditions.end());
        }

        //part 1
        //auto sum = ProcessNext(0l, springsChunk, 0l, conditions);        

        //part 2
        std::vector<std::vector<std::vector<long long>>> memoTable;
        memoTable.resize(5);
        for (long long i = 0; i < 5; i++)
        {
            memoTable[i].resize(expandedSpringsInput.size());
            for (long long j = 0; j < expandedSpringsInput.size(); j++)
            {
                memoTable[i][j].resize(expandedConditions.size() +1, -1ll);
            }
        }
        auto sum = ProcessNext(memoTable, 0ll, expandedSpringsInput, 0ll, expandedConditions);
        totalArrangementSum += sum;
    }

    ifs.clear();

    std::cout << "total arrangement sum: " << totalArrangementSum<< '\n';

    return 0;
}