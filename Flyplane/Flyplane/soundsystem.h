#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "soundcomponent.h"
#include "transform.h"
#include "playercomponent.h"
#include "flightcomponent.h"
#include "input.h"
#include "assetloader.h"
#include "timer.h"
#include "globaltimer.h"
#include "pauseEvent.h"

using namespace entityx;

struct SoundSystem : public System<SoundSystem>, public Receiver<SoundSystem> {
private:
	sf::Sound driftSound;
public:
	SoundSystem() {
		driftSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("wind"));
		driftSound.setRelativeToListener(true);
		driftSound.setPosition(0, 0, 0);
	}

	void configure(EventManager &eventManager) override {
		eventManager.subscribe<PauseEvent>(*this);
	}

	void receive(const PauseEvent &event) {
		if (event.pause) {
			if (driftSound.getStatus() == sf::Sound::Playing)
				driftSound.pause();

			ComponentHandle<SoundComponent> sound;
			for (Entity entity : event.ex.entities.entities_with_components(sound)) {
				if (sound->sound.getStatus() == sf::Sound::Playing) {
					sound->sound.pause();
				}
			}

			ComponentHandle<BurstSoundComponent> burstSound;
			for (Entity entity : event.ex.entities.entities_with_components(burstSound)) {
				BurstSoundComponent* s = burstSound.get();

				for (auto& sound : s->sounds) {
					if (sound.getStatus() == sf::Sound::Playing) {
						sound.pause();
					}
				}
			}
		}
		else {
			if (driftSound.getStatus() == sf::Sound::Paused)
				driftSound.play();

			ComponentHandle<SoundComponent> sound;
			for (Entity entity : event.ex.entities.entities_with_components(sound)) {
				if (sound->sound.getStatus() == sf::Sound::Paused) {
					sound->sound.play();
				}
			}

			ComponentHandle<BurstSoundComponent> burstSound;
			for (Entity entity : event.ex.entities.entities_with_components(burstSound)) {
				BurstSoundComponent* s = burstSound.get();

				for (auto& sound : s->sounds) {
					if (sound.getStatus() == sf::Sound::Paused) {
						sound.play();
					}
				}
			}
		}
	}

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		ComponentHandle<SoundComponent> sound;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(sound, transform)) {
			sound = entity.component<SoundComponent>();
			transform = entity.component<Transform>();

			SoundComponent* s = sound.get();
			Transform t = *transform.get();

			s->sound.setPosition(t.pos.x, t.pos.y, t.pos.z);
		}

		ComponentHandle<BurstSoundComponent> burstSound;
		for (Entity entity : es.entities_with_components(burstSound, transform)) {
			burstSound = entity.component<BurstSoundComponent>();
			transform = entity.component<Transform>();

			BurstSoundComponent* s = burstSound.get();
			Transform* t = transform.get();

			for (auto& sound : s->sounds) {
				sound.setPosition(t->pos.x, t->pos.y, t->pos.z);
			}
			/*if ((Input::isKeyDown(GLFW_KEY_LEFT_CONTROL) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER))) {
				//s->sound.stop();
				if (s->sound.getStatus() != s->sound.Playing) {
					s->sound.play();
					//s->sound.setLoop(true);
				}
			}*/
			//else if (!(Input::isKeyDown(GLFW_KEY_LEFT_CONTROL) || Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER))) {
				//s->sound.stop();
			//}
		}
		
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<FlightComponent> flyplane;
		for (Entity entity : es.entities_with_components(player, transform)) {
			sound = entity.component<SoundComponent>();
			transform = entity.component<Transform>();
			flyplane = entity.component<FlightComponent>();
			

			Transform t = *transform.get();

			if (flyplane && sound) {
				SoundComponent* s = sound.get();
				FlightComponent* f = flyplane.get();
				float pitch = 0;

				pitch = glm::clamp((f->throttle - f->airBrake), -0.2f, 1.f) * 0.8f + 1.0f;
				s->sound.setPitch(pitch);

				//std::cout << "Drift: " << f->drift << std::endl;
				if (f->drift > 0.2) {
					if (driftSound.getStatus() != driftSound.Playing) {
						driftSound.play();
					}
				}
				else {
					driftSound.stop();
				}
			}

			//s.sound.setPosition(t.pos.x, t.pos.y, t.pos.z);
			sf::Listener::setPosition(t.pos.x, t.pos.y, t.pos.z);
			glm::vec3 vec = glm::toMat3(t.orientation) * glm::vec3(0.0, 0.0, 1.0);
			sf::Listener::setDirection(vec.x, vec.y, vec.z);
			vec = glm::toMat3(t.orientation) * glm::vec3(0.0, 1.0, 0.0);
			sf::Listener::setUpVector(vec.x, vec.y, vec.z);
		}

		if (Input::isKeyPressed(GLFW_KEY_M)) {
			sf::Listener::setGlobalVolume(sf::Listener::getGlobalVolume() + 1);
			std::cout << "Volume: " << sf::Listener::getGlobalVolume() << std::endl;
		}
		if (Input::isKeyPressed(GLFW_KEY_N)) {
			sf::Listener::setGlobalVolume(sf::Listener::getGlobalVolume() - 1);
			std::cout << "Volume: " << sf::Listener::getGlobalVolume() << std::endl;
		}
	}
};