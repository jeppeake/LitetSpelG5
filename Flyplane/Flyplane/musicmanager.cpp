#include "musicmanager.h"

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
	music.openFromFile(songFile);
	music.play();
}

void MusicManager::pause() {
	music.pause();
}

void MusicManager::stop() {
	music.stop();
}
