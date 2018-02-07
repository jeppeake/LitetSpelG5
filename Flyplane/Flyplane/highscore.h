#pragma once

#include<fstream>
#include <string>
#include <vector>

using namespace std;

struct HighscoreElement {
	int score = 0;
	string name = "";
};


class Highscore {
private:
	int nrOfElements = 0;
	HighscoreElement list[10];
	string highscoreList[10];
	void convertListToString();
public:
	Highscore(const Highscore &other) = delete;
	static Highscore& getHighscore()
	{
		static Highscore s;
		return s;
	}
	Highscore();
	~Highscore();
	string* getHighscoreList();
	void addScore(string name, int score);
	void writeToFile() const;
}; 