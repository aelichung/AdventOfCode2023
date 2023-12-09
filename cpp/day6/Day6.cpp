#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

struct Race {
	long long duration, distance;

	bool IsAWin(int timeHeld) {
		int speed = timeHeld;
		int remainingTime = duration - timeHeld;
		if (remainingTime <= 0) return 0;

		return remainingTime * speed > distance;
	}

	std::vector<long long> FindTimeForDistance() {				
		// negative b + or - square root of b^2 - 4ac
		long long squareRoot = sqrt(pow(duration, 2) - (4 * 1 * distance));
		long long plus = duration + squareRoot - 1;
		long long minus = duration - squareRoot  ;

		//divide by 2a
		std::vector<long long> times;
		times.push_back((long long)std::ceil(minus / 2));
		times.push_back((long long) std::floor(plus / 2));
		return { times  };
	}

	long long NumWaysToWin() {
		long long minIndex, maxIndex;
		//start in the middle and subdivide the start everytime we get a winning Distance
		long long halfIndex = (long long)std::floor(duration / 2);
		long long previousIndex = 0;
		while (IsAWin(halfIndex)) {		
			previousIndex = halfIndex;
			halfIndex = (long long)std::floor(halfIndex / 2);
		}
		//at this point the index at half index is not a win,
		//our bounds lies somewhere between current half index and the previous half index
		//where previous index is a win and current half index is not a win
		long long winIndex = previousIndex;
		long long failIndex = halfIndex;
		while (winIndex - failIndex > 1) {
			int diff = winIndex - failIndex;
			failIndex = failIndex + (long long)std::floor(diff / 2);

			if (IsAWin(failIndex)) {
				//we need to search for our boundary between this half index and our previous half index
				winIndex = failIndex;
				failIndex = halfIndex;
			}
		}

		if (IsAWin(winIndex - 1)) {
			minIndex = winIndex - 1;
		}
		else {
			minIndex = winIndex;
		}

		//reset half index
		halfIndex = (long long)std::floor(duration / 2);
		long long endIndex = duration - 1;
		long long previousEndIndex = 0;
		while (!IsAWin(endIndex))
		{
			int diff = endIndex - halfIndex;
			previousEndIndex = endIndex;
			endIndex = endIndex - (long long)std::floor(diff / 2);
		}
		failIndex = previousEndIndex;
		winIndex = endIndex;

		while (failIndex - winIndex > 1) {
			int diff = failIndex - winIndex;
			failIndex = failIndex - (long long)std::floor(diff / 2);

			if (IsAWin(failIndex)) {
				winIndex = failIndex;
				failIndex = previousEndIndex;
			}
		}

		if (IsAWin(winIndex + 1)) {
			maxIndex = winIndex + 1;
		}
		else {
			maxIndex = winIndex;
		}

		return maxIndex - minIndex +1;
	}
};

std::ostream& operator<<(std::ostream& os, const Race& race) {
	os << "duration: " << race.duration << ", distance: " << race.distance << std::endl;
	return os;
}

int main() {

	std::ifstream ifs("inputs/day6/input-test.txt", std::ifstream::in);
	std::string s;
	std::regex valPattern { R"( (\d+))" };
	//part 1: std::vector<Race> races;

	std::getline(ifs, s);
	std::sregex_token_iterator durationIterator(s.begin(), s.end(), valPattern, 1);
	std::sregex_token_iterator iteratorEnd;

	std::string durationString;
	Race race;
	while (durationIterator != iteratorEnd) {		
		durationString += *durationIterator;
		durationIterator++;
	}

	long long duration = std::stoll(durationString);
	race.duration = duration;

	std::getline(ifs, s);
	std::sregex_token_iterator distanceIterator(s.begin(), s.end(), valPattern, 1);
	
	std::string distanceString;
	while (distanceIterator != iteratorEnd) {
		distanceString += *distanceIterator;
		distanceIterator++;
	}

	long long distance = std::stoll(distanceString);
	race.distance = distance;

	ifs.clear();	

	std::vector<long long> times = race.FindTimeForDistance();
	long long diff = times.at(1) - times.at(0);

	Race testRace(30, 200);
	std::vector<long long> testTimes = testRace.FindTimeForDistance();
	std::cout << "time held 1: " << testTimes.at(0) << ", time held 2: " << testTimes.at(1) << std::endl;

	std::cout << "number ways to win long ass race: "<< diff;

	return 0;

}