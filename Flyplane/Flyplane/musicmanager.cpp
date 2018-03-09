#include "musicmanager.h"
#include <iostream>

MusicManager::MusicManager() {
	music.setLoop(true);
	music.setRelativeToListener(true);
	music.setPosition(0.0f, 0.0f, 0.0f);
	music.setVolume(50);
	offset[0] = 0;
	offset[1] = 195;
	offset[2] = 403;
	offset[3] = 611;
	offset[4] = 828;
	offset[5] = 1096;
	offset[6] = 1388;
	offset[7] = 1603;
	offset[8] = 1874;
	offset[9] = 2100;
	offset[10] = 2298;
	offset[11] = 2489;
	offset[12] = 2676;
	offset[13] = 2982;
	offset[14] = 3151;
}

void MusicManager::play() {
	music.play();
}

void MusicManager::play(std::string songFile) {
	bool worked = music.openFromFile(songFile);
	if (!worked)
		std::cout << "could not load " + songFile << std::endl;
	music.play();
}

void MusicManager::playMenuMusic() {
	bool worked = music.openFromFile("Assets/Sound/menumusic.ogg");
	if (!worked)
		std::cout << "could not load menumusig.ogg" << std::endl;
	index = rand() % 15;
	music.play();
	music.setPlayingOffset(sf::seconds(offset[index]));
}

void MusicManager::changeSong() {
	auto t = music.getPlayingOffset();

	int b = (int)t.asSeconds();
	if (offset[index] > b) {
		index = 0;
	}

	while (offset[index] <= b) {
		index++;
		if (index == 15) {
			index = 0;
			break;
		}
		std::cout << index << std::endl;
	}
	music.setPlayingOffset(sf::seconds(offset[index]));
}

void MusicManager::pause() {
	music.pause();
}

void MusicManager::stop() {
	music.stop();
}
