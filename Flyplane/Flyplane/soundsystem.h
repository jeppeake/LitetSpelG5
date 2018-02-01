#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "soundcomponent.h"
#include "transform.h"
#include "playercomponent.h"

using namespace entityx;

struct SoundSystem : public System<SoundSystem> {

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		ComponentHandle<SoundComponent> sound;
		ComponentHandle<Transform> transform;

		for (Entity entity : es.entities_with_components(sound, transform)) {
			sound = entity.component<SoundComponent>();
			transform = entity.component<Transform>();

			SoundComponent s = *sound.get();
			Transform t = *transform.get();

			s.sound.setPosition(t.pos.x, t.pos.y, t.pos.z);
		}
		
		ComponentHandle<PlayerComponent> player;
		for (Entity entity : es.entities_with_components(player, sound, transform)) {
			sound = entity.component<SoundComponent>();
			transform = entity.component<Transform>();

			SoundComponent s = *sound.get();
			Transform t = *transform.get();

			s.sound.setPosition(t.pos.x, t.pos.y, t.pos.z);
			sf::Listener::setPosition(t.pos.x, t.pos.y, t.pos.z);
			glm::vec3 vec = glm::toMat3(t.orientation) * glm::vec3(0.0, 0.0, 1.0);
			sf::Listener::setDirection(vec.x, vec.y, vec.z);
			vec = glm::toMat3(t.orientation) * glm::vec3(0.0, 1.0, 0.0);
			sf::Listener::setUpVector(vec.x, vec.y, vec.z);
		}
	}
};