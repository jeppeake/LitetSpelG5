#pragma once
#include <sfml\Audio.hpp>

struct SoundComponent {
	SoundComponent(sf::SoundBuffer& buffer, float minDistance = 10) {
		sound.setBuffer(buffer);
		sound.play();
		sound.setLoop(true);
		sound.setMinDistance(minDistance);
		sound.setAttenuation(1);
		sound.setVolume(50);
	}

	sf::Sound sound;
};

struct BurstSoundComponent {
	BurstSoundComponent(sf::SoundBuffer& buffer, glm::vec3 pos = glm::vec3(0), bool shouldPlay = false, float minDistance = 50.f, float attenuation = 1.1f) {
		sound.setBuffer(buffer);
		sound.setLoop(false);
		sound.setMinDistance(minDistance);
		sound.setAttenuation(attenuation);
		sound.setVolume(50);
		sound.setPosition(pos.x, pos.y, pos.z);
		
		if(shouldPlay)
			sound.play();
	}

	sf::Sound sound;
};