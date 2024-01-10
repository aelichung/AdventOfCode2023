#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <map>
#include <algorithm>
#include <set>

//std::map<char, int> CardType = { {'2', 0}, {'3', 1}, {'4',2}, {'5',3}, {'6',4}, {'7',5}, {'8',6}, {'9',7}, {'T',8}, {'J',9}, {'Q',10}, {'K',11}, {'A',12} };

constexpr int GetCardValue(char c) {
	if (c >= '2' && c <= '9') {
		return c - '2';
	}
	else if (c == 'T') {
		return 8;
	}
	else if (c == 'J') {
		return -1;
	}
	else if (c == 'Q') {
		return 10;
	}
	else if (c == 'K') {
		return 11;
	}
	else if (c == 'A') {
		return 12;
	}
	return -2;
}

const int NUMCARDSINHAND = 5;

struct Card {
	char cardType;

	friend constexpr bool operator==(const Card& l, const Card& r) noexcept 
	{
		return l.cardType == r.cardType;
	}
	friend constexpr auto  operator<=>(const Card& l, const Card& r) noexcept
	{
		int leftVal = GetCardValue(l.cardType);
		int rightVal = GetCardValue(r.cardType);
		return leftVal <=> rightVal;
	}
};



enum HandType {
	FiveOfAKind = 6,
	FourOfAKind = 5, 
	FullHouse =   4, 
	ThreeOfAKind =3,
	TwoPair =     2,
	OnePair =     1,
	HighCard =    0
};

std::ostream& operator<<(std::ostream& os, const HandType& handType) {
	std::string printString;
	if (handType == FiveOfAKind) {
		printString = "Five of a Kind";
	}
	if (handType == FourOfAKind) {
		printString = "Four of a Kind";
	}
	if (handType == FullHouse) {
		printString = "Fullhouse";
	}
	if (handType == ThreeOfAKind) {
		printString = "Three of a Kind";
	}
	if (handType == TwoPair) {
		printString = "Two Pair";
	}
	if (handType == OnePair) {
		printString = "One Pair";
	}
	if (handType == HighCard) {
		printString = "High Card";
	}

	os << printString;
	return os;
}

struct Hand {
	std::vector<Card> cards;
	int bid;
	HandType type;

	Hand(std::vector<Card> cards, int bid):cards(cards), bid(bid) {
		type = FindType();
	}
	
	HandType FindType() {
		std::vector<int> numMatches;
		std::set<char> checked;
		int numberOfJ = 0;
		for (int i = 0; i < NUMCARDSINHAND; i++) {			
			char c = cards[i].cardType;
			if (c == 'J') ++numberOfJ;
			if (checked.contains(c)) continue;
			checked.insert(c);
			int cardCount = 1;
			for (int j = i + 1; j < NUMCARDSINHAND; j++) {				
				if (c == cards[j].cardType) {
					cardCount++;
				}
			}
			if (cardCount > 1) {
				numMatches.push_back(cardCount);
			}
		}
		for (int countIndex = 0; countIndex < numMatches.size(); countIndex++) {
			if (numMatches[countIndex] == 5) {
				return FiveOfAKind;
			}
			if (numMatches[countIndex] == 4) {
				if (numberOfJ == 1 || numberOfJ == 4) return FiveOfAKind;
				return FourOfAKind;
			}
			if (numMatches[countIndex] == 3 && numMatches.size() == 2) {
				if (numberOfJ == 3 || numberOfJ == 2) return FiveOfAKind;
				return FullHouse;
			}
			if (numMatches[countIndex] == 3 && numMatches.size() == 1) {
				if (numberOfJ == 1 || numberOfJ == 3) return FourOfAKind;				
				return ThreeOfAKind;
			}
			if (numMatches[countIndex] == 2 && numMatches.size() == 1) {								
				if (numberOfJ == 1 || numberOfJ == 2) return ThreeOfAKind;
				return OnePair;
			}
			if (numMatches[countIndex] == 2 && numMatches[countIndex + 1] == 2) {
				if (numberOfJ == 1) return FullHouse;
				if (numberOfJ == 2) return FourOfAKind;				
				return TwoPair;
			}
		}
		if (numberOfJ == 1) {
			return OnePair;
		}
		return HighCard;
	}

	friend constexpr bool operator==(const Hand& l, const Hand& r) noexcept
	{
		if (l.type != r.type) {
			return false;
		}
		for (int i = 0; i < NUMCARDSINHAND; i++) {
			if (l.cards[i] != r.cards[i]) {
				return false;
			}
		}
		return true;
	}
	friend constexpr auto operator<=>(const Hand& l, const Hand& r) noexcept
	{
		if (l.type != r.type) {
			return l.type <=> r.type;
		}

		int cardIndex = 0;
		while (l.cards[cardIndex] == r.cards[cardIndex]) {
			cardIndex++;
		}
		
		return l.cards[cardIndex] <=> r.cards[cardIndex];
	}
};

std::ostream& operator<<(std::ostream& os, const Hand& hand) {
	std::string handString;
	for (auto& card : hand.cards) {
		handString += card.cardType;
	}
	os << "cards: " << handString << ", bid: " << hand.bid << ", handType: " << hand.type<<std::endl;
	return os;
}



int main() {
	std::ifstream ifs("inputs/day7/input.txt", std::ifstream::in);
	//std::ifstream ifs("inputs/day7/input-test.txt", std::ifstream::in);
	std::string s;
	std::vector<Hand> hands;
	std::regex handPattern{ R"((\w+) (\d+))" };

	while (ifs.good()) {
		std::getline(ifs, s);

		std::smatch smatch;
		bool test = std::regex_search(s, smatch, handPattern);
		std::string handString = smatch[1];
		std::vector<Card> cards;

		for (int i = 0; i < NUMCARDSINHAND; i++) {
			Card card(handString[i]);
			cards.push_back(card);
		}

		int bid = std::stoi(smatch[2]);

		Hand hand(cards, bid);
		hands.push_back(hand);
	}
	ifs.close();

	std::cout << "number of hands: " << hands.size()<<std::endl;

	std::sort(hands.begin(), hands.end());

	long long sum = 0;
	for (int i = 1; i <= hands.size(); i++) {
		long long winnings = i * hands[i - 1].bid;
		sum += winnings;
		std::cout <<"rank "<<i<<": "<< hands[i - 1] << "winnings: "<<winnings<< std::endl;
	}

	std::cout << "sum: " << sum;

	return 0;
}