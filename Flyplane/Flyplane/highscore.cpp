#include "highscore.h"
#include<fstream>
#include <iostream>


void Highscore::convertListToString() {
	for (int i = 0; i < nrOfElements; i++) {
		highscoreList[i] = list[i].name;
		string temp = to_string(list[i].score);
		for (int k = 0; k < 40 - list[i].name.size() - temp.size(); k++) {
			highscoreList[i] += " ";
		}
		highscoreList[i] += temp;
	}
}

Highscore::Highscore() {
	ifstream file;
	file.open("highscorelist.txt");

	if (!file.is_open()) {
		cout << "highscorelist unable to open" << endl;
	}
	string name;
	int score;

	while (file >> score) {
		file >> name;

		list[nrOfElements] = {score, name};
		nrOfElements++;
	}
		
}

Highscore::~Highscore() {
	writeToFile();
}

string* Highscore::getHighscoreList() {
	return highscoreList;
}

void Highscore::addScore(string name, int score) {
	HighscoreElement temp;
	if (nrOfElements < 10) {
		nrOfElements++;
	}
	for (int i = 0; i < nrOfElements; i++) {
		if (list[i].score < score) {
			temp = list[i];
			list[i] = { score, name};
			score = temp.score;
			name = temp.name;
		}
	}

	convertListToString();
}

void Highscore::writeToFile() const {
	ofstream file;
	file.open("highscorelist.txt");

	if (file.is_open()) {
		for (int i = 0; i < nrOfElements; i++) {
			file << list[i].score << endl;
			file << list[i].name << endl;
		}
	}
	file.close();
}
