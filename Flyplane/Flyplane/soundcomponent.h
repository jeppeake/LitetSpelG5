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