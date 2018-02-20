#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "soundcomponent.h"
#include "transform.h"
#include "playercomponent.h"
#include "input.h"

using namespace entityx;

struct SoundSystem : public System<SoundSystem> {

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

			s->sound.setPosition(t->pos.x, t->pos.y, t->pos.z);
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
		for (Entity entity : es.entities_with_components(player, transform)) {
			sound = entity.component<SoundComponent>();
			transform = entity.component<Transform>();

			SoundComponent* s = sound.get();
			Transform t = *transform.get();

			if (Input::isKeyDown(GLFW_KEY_W)) {
				s->sound.setPitch(s->sound.getPitch() + 0.001f);
			}
			else if (Input::isKeyDown(GLFW_KEY_S)) {
				s->sound.setPitch(s->sound.getPitch() - 0.001f);
			}
			//std::cout << "Pitch: " << s->sound.getPitch() << std::endl;
			//s.sound.setPosition(t.pos.x, t.pos.y, t.pos.z);
			sf::Listener::setPosition(t.pos.x, t.pos.y, t.pos.z);
			glm::vec3 vec = glm::toMat3(t.orientation) * glm::vec3(0.0, 0.0, 1.0);
			sf::Listener::setDirection(vec.x, vec.y, vec.z);
			vec = glm::toMat3(t.orientation) * glm::vec3(0.0, 1.0, 0.0);
			sf::Listener::setUpVector(vec.x, vec.y, vec.z);
		}
	}
};