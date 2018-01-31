#pragma once
#include <sfml\Audio.hpp>

struct SoundComponent {
	SoundComponent(sf::SoundBuffer& buffer) {
		sound.setBuffer(buffer);
		sound.play();
	}

	sf::Sound sound;
};