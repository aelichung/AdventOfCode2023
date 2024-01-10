#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <regex>
#include <map>
#include <numeric>

class Instructions {
public:
    int currentIndex = 0;
    Instructions(std::string instructionLine) : instructionLine(instructionLine) {

    }

    char GetNextStep() {
        char toReturn = instructionLine[currentIndex];
        currentIndex++;
        if (currentIndex >= instructionLine.length())
            currentIndex = 0;

        return toReturn;
    }

private:
    std::string instructionLine;
};

class Node {
public:
    std::string value;
    std::string left;
    std::string right;
    Node() {}

    Node(std::string value, std::string left, std::string right) : value(value), left(left), right(right) {}
};

class NodeMap {
public:
    std::map<std::string, Node> nodes;
};

class GhostRunner {
public:
    Node currentNode;
    Instructions instructions;
    NodeMap nodeMap;
    int stepsToZ;

    GhostRunner(Node node, NodeMap map, Instructions instructions) : 
        currentNode(node), nodeMap(map), instructions(instructions) {};

    bool EndsWithZ() {
        return currentNode.value[2] == 'Z';
    }

    void Step() {
        char nextStep = instructions.GetNextStep();
        if (nextStep == 'L') {
            std::string nextNode = currentNode.left;
            currentNode = nodeMap.nodes[nextNode];            
        }
        else {
            std::string nextNode = currentNode.right;
            currentNode = nodeMap.nodes[nextNode];            
        }
    }
};

int main()
{
    std::cout << "Advent of Code 2023 - Day 08!\n";

    std::ifstream ifs("inputs/day8/input.txt");

    // Parse instructions
    std::string instructionsString;
    std::getline(ifs, instructionsString); // Instruction line
    Instructions instructions(instructionsString);

    // Parse node map
    NodeMap nodeMap;
    std::string mapLine;
    std::getline(ifs, mapLine);
    std::regex nodeLinePattern(R"((\w\w\w) = \((\w\w\w), (\w\w\w))");
    while (std::getline(ifs, mapLine)) {
        std::smatch match;
        bool matchSuccess = std::regex_search(mapLine, match, nodeLinePattern);
        if (!matchSuccess)
            std::cout << "Regex failure!!!" << std::endl;

        Node newNode(match[1], match[2], match[3]);
        nodeMap.nodes[match[1]] = newNode;
    }

    //// Start walking the node map
    //Node currentNode = nodeMap.nodes["AAA"];
    //int totalSteps = 0;
    //while (currentNode.value != "ZZZ") {
    //    char nextStep = instructions.GetNextStep();
    //    if (nextStep == 'L') {
    //        std::string nextNode = currentNode.left;
    //        currentNode = nodeMap.nodes[nextNode];
    //        totalSteps++;
    //    }
    //    else {
    //        std::string nextNode = currentNode.right;
    //        currentNode = nodeMap.nodes[nextNode];
    //        totalSteps++;
    //    }
    //}

    //std::cout << "PART 1 ANSWER - Total step count: " << totalSteps << std::endl;

    std::vector<GhostRunner> ghosts;
    //part 2
    for (auto& valueNodePair : nodeMap.nodes) {
        if (valueNodePair.first[2] == 'A') {
            auto ghost = GhostRunner(valueNodePair.second, nodeMap, instructions);
            ghosts.push_back(ghost);
        }
    }

    std::cout << "number of ghosts: " << ghosts.size()<< std::endl;
    std::vector<int> stepCounts;
    
    for (auto& ghost : ghosts) {
        int stepCount = 0;
        while (!ghost.EndsWithZ()) {
            ghost.Step();
            ++stepCount;           
        }
        stepCounts.push_back(stepCount);
        std::cout << stepCounts.size() << ": " << stepCount << std:: endl;
        ghost.stepsToZ = stepCount;
    }  


    //stepCounts = { 21,14,38 };
    auto lcm = std::accumulate(std::next(stepCounts.begin()), stepCounts.end(), (unsigned long long)stepCounts[0], [](unsigned long long a, unsigned long long b)
    {
        std::cout << a << ", " << b << std::endl;
        return std::lcm(a, b);
    });

    
    std::cout << "PART 2 ANSWER - Total step count: " << lcm << std::endl;
}