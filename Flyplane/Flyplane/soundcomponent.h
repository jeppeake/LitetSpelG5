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
	BurstSoundComponent(sf::SoundBuffer& buffer, glm::vec3 pos = glm::vec3(0), bool shouldPlay = false, float minDistance = 50.f, float attenuation = 1.1f, size_t numSounds = 3, float volume = 50) {
		for (int i = 0; i < numSounds; i++) {
			sounds.emplace_back();
			sounds.back().setBuffer(buffer);
			sounds.back().setLoop(false);
			sounds.back().setMinDistance(minDistance);
			sounds.back().setAttenuation(attenuation);
			sounds.back().setVolume(volume);
			sounds.back().setPosition(pos.x, pos.y, pos.z);
		
			if(shouldPlay)
				sounds.back().play();
		}
	}

	std::vector<sf::Sound> sounds;
};