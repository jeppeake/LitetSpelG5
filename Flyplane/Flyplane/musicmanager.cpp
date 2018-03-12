#include "musicmanager.h"
#include <iostream>
#include "input.h"
#include <GLFW/glfw3.h>

MusicManager::MusicManager() {
	music.setLoop(false);
	music.setRelativeToListener(true);
	music.setPosition(0.0f, 0.0f, 0.0f);
	playing = false;
}

void MusicManager::update() {
	if (!playing) {
		auto status = music.getStatus();
		if (status == music.Stopped)
			changeSong();

		if (Input::isKeyPressed(GLFW_KEY_TAB))
			MusicManager::getMusicManager().changeSong();
	}
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

void MusicManager::playPlayingMusic() {
	playing = true;
	bool worked = music.openFromFile("Assets/Sound/playingmusic.wav");
	if (!worked)
		std::cout << "could not load playingmusic.wav" << std::endl;
	music.play();
}

void MusicManager::playMenuMusic() {
	music.pause();
	playing = false;

	if (playlist.size() > 0) {
		index = rand() % playlist.size();
		play(playlist[index]);
	}
	
}

void MusicManager::addSongToPlayList(string name) {
	playlist.push_back(name);
}

void MusicManager::changeSong() {
	if (playlist.size() > 0) {
		index++;
		index = index % playlist.size();
		play(playlist[index]);
	}
}

void MusicManager::pause() {
	music.pause();
}

void MusicManager::stop() {
	music.stop();
}
