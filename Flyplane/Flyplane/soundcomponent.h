#pragma once
#include <sfml\Audio.hpp>

struct SoundComponent {
	SoundComponent(sf::SoundBuffer& buffer) {
		sound.setBuffer(buffer);
		sound.play();
		sound.setLoop(true);
		sound.setMinDistance(100);
		sound.setAttenuation(1);
	}

	sf::Sound sound;
};

struct BurstSoundComponent {
	BurstSoundComponent(sf::SoundBuffer& buffer) {
		sound.setBuffer(buffer);
		//sound.play();
		sound.setLoop(false);
		sound.setMinDistance(100);
		sound.setAttenuation(1);
	}

	sf::Sound sound;
};