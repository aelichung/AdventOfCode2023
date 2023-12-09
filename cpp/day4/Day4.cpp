#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h> //pow

class Card {
public:
	std::vector<int> winningNumbers, actualNumbers;
	int copies = 1;
	
	int GetActualWins() {
		int actualWins = 0;
		for (int i = 0; i < actualNumbers.size(); i++) {
			int actualNum = actualNumbers[i];
			if (std::find(winningNumbers.begin(), winningNumbers.end(), actualNum) != winningNumbers.end()) {
				actualWins++;
			}
		}
		return actualWins;
	}

	int GetPointValue() {			
		int points = pow(2, GetActualWins()-1);
		return points;
	}
};

int main() {

	std::ifstream ifs("input_Day4.txt", std::ifstream::in);
	std::string s;

	std::vector<Card> cards;


	while (ifs.good()) {
		std::getline(ifs, s);

		int colonPos = s.find(':');
		s.erase(0, colonPos + 2);
		//std::cout << "erased s: " << s << '\n';

		int pipePos = s.find('|');
		std::string winningString = s.substr(0, pipePos);
		//std::cout << "winnning string: " << winningString << '\n';
		s.erase(0, pipePos + 2);

		//std::cout << "erased s: " << s << '\n';

		Card card;

		while (s.length() > 0) {
			int num = std::stoi(s.substr(0, 3));			
			card.actualNumbers.push_back(num);			
			s.erase(0, 3);
		}

		while (winningString.length() > 0) {
			int num = std::stoi(winningString.substr(0, 3));
			card.winningNumbers.push_back(num);
			winningString.erase(0, 3);
		}
		
		cards.push_back(card);
	}
	//part 1!
	//int sum = 0;
	//for (auto& card : cards) {
	//	std::cout <<"card wins: "<<card.GetActualWins()<<", point value : " << card.GetPointValue() << '\n';
	//	sum += card.GetPointValue();
	//}
	//std::cout << "sum: " << sum << '\n';

	for (int activeCardIndex = 0; activeCardIndex < cards.size(); activeCardIndex++) {
		Card activeCard = cards[activeCardIndex];
		int wins = activeCard.GetActualWins();
		for (int effectedCardIndex = activeCardIndex + 1; effectedCardIndex <= activeCardIndex + wins && effectedCardIndex < cards.size(); effectedCardIndex++) {
			cards[effectedCardIndex].copies += activeCard.copies;
		}
	}

	int cardSum = 0;
	for (auto& card : cards) {
		cardSum += card.copies;
	}

	std::cout << "card sum: " << cardSum;

	return 0;
}