#pragma once
#include <sfml\Audio.hpp>

struct SoundComponent {
	SoundComponent(sf::SoundBuffer& buffer, bool shouldLoop) {
		sound.setBuffer(buffer);
		sound.play();
		sound.setLoop(shouldLoop);
		sound.setMinDistance(100);
		sound.setAttenuation(1);
	}

	sf::Sound sound;
};