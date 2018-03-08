#include "musicmanager.h"
#include <iostream>
MusicManager::MusicManager() {
	music.setLoop(true);
	music.setRelativeToListener(true);
	music.setPosition(0.0f, 0.0f, 0.0f);
	music.setVolume(50);
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

void MusicManager::pause() {
	music.pause();
}

void MusicManager::stop() {
	music.stop();
}
