#pragma once
#include <SFML\Audio\Music.hpp>
#include <string>

using namespace std;

class MusicManager {
private:
	sf::Music music;
	std::vector<string> playlist;
	int index;
	bool playing;
public:
	MusicManager();
	MusicManager(const MusicManager &other) = delete;
	static MusicManager& getMusicManager()
	{
		static MusicManager s;
		return s;
	}
	void update();
	void play();
	void play(std::string songFile);
	void playPlayingMusic();
	void playMenuMusic();
	void addSongToPlayList(string name);
	void changeSong();
	void pause();
	void stop();
};
