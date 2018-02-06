#pragma once

#include<fstream>
#include <string>
#include <vector>

using namespace std;

struct HighscoreElement {
	int score;
	string name;
};


class Highscore {
private:
	vector<HighscoreElement> list;
	string highscoreList[10];
	void convertListToString();
public:
	Highscore();
	~Highscore();
	string* getHighscoreList();
	void addScore(string name, int score);
}; 