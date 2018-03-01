#pragma once
#include "healthcomponent.h"
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include <iostream>
#include "pointcomponent.h"
#include "playingstate.h"

using namespace entityx;

struct HealthSystem : public entityx::System<HealthSystem> {
	sf::Sound explosionSound;

	PlayingState* state;
	HealthSystem(PlayingState *state) : state(state) {
		explosionSound.setBuffer(*AssetLoader::getLoader().getSoundBuffer("explosion"));
		explosionSound.setRelativeToListener(true);
		explosionSound.setPosition(0, 0, 0);
	};
	entityx::ComponentHandle<HealthComponent> health;
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		for (Entity entity : es.entities_with_components(health)) {
			if (health->health <= 0 && !health->isDead) {
				health->isDead = true;
				std::cout << "Entity got killed." << "\n";
				if (entity.has_component<PointComponent>())
					state->addPoints(entity.component<PointComponent>()->points);
				if (entity.has_component<FlightComponent>())
				{
					entity.remove<FlightComponent>();
					if (entity.has_component<Target>())
					{
						entity.remove<Target>();
					}
					if (entity.has_component<SoundComponent>()) {
						entity.remove<SoundComponent>();
					}
				}

				if (entity.has_component<PlayerComponent>()) {
					entity.assign<LifeTimeComponent>(1.5f);
					explosionSound.play();
				}

				auto handle = entity.component<ParticleComponent>();
				if (!handle)
				{
					handle = entity.assign<ParticleComponent>();
				}
				ParticleParameters p;
				p.effectLength = 3.f;
				p.explosion.radius = 50.f;
				events.emit<AddParticleEvent>(EXPLOSION, handle, p);
				events.emit<RemoveParticleEvent>(TRAIL, handle);
				events.emit<RemoveParticleEvent>(ENGINE_TRAIL, handle);
				events.emit<AddParticleEvent>(DEAD_TRAIL, handle);
			}
		}
	}
};