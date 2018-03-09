#pragma once
#include <SFML\Audio\Music.hpp>
#include <string>

class MusicManager {
private:
	sf::Music music;
	int offset[15];
	int index;
public:
	MusicManager();
	MusicManager(const MusicManager &other) = delete;
	static MusicManager& getMusicManager()
	{
		static MusicManager s;
		return s;
	}
	void play();
	void play(std::string songFile);
	void playMenuMusic();
	void changeSong();
	void pause();
	void stop();
};
