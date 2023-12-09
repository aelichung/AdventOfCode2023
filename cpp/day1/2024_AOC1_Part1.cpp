// 2024_AoC1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>      // std::ifstream

int main()
{

   
    std::ifstream ifs("input_John.txt", std::ifstream::in);

    char* s = (char*)malloc(256 * sizeof(char));
    int sum = 0;
    ifs.getline(s, 256);
    while (ifs.good()) {        
        int i = 0;
        char firstChar = s[i];
        bool isDigit = std::isdigit(firstChar);
        while (firstChar != '\0' && !isDigit) {
            i++;
            firstChar = s[i];
            isDigit = std::isdigit(firstChar);
        }
        

        int j = ifs.gcount() -1;
        char lastChar = s[j];
        isDigit = std::isdigit(lastChar);

        while (j >= 0 && !isDigit) {
            j--;
            lastChar = s[j];
            isDigit = std::isdigit(lastChar);
        }

        std::cout << "given " << s << ", found first digit " << firstChar << ", found last digit " <<lastChar<< "\n";
        char numberString[] = { firstChar, lastChar, '\0' };
        int number = atoi(numberString);
        sum += number;
        ifs.getline(s, 256);
    }

    ifs.close();
    std::cout << "final sum: " << sum;
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
