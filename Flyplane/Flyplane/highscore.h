#pragma once

#include<fstream>
#include <string>

using namespace std;

class Highscore {
private:
	string list[5];
public:
	Highscore();
	~Highscore();
	string* getHighscoreList();
}; 