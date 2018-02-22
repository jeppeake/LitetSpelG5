#pragma once
#include "healthcomponent.h"
#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include <iostream>
#include "pointcomponent.h"
#include "playingstate.h"

using namespace entityx;

struct HealthSystem : public entityx::System<HealthSystem> {
	PlayingState* state;
	HealthSystem(PlayingState *state) : state(state) {};
	entityx::ComponentHandle<HealthComponent> health;
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {
		for (Entity entity : es.entities_with_components(health)) {
			if (health->health <= 0 && !health->isDead) {
				health->isDead = true;
				std::cout << "Entity got killed." << "\n";
				if (entity.has_component<PointComponent>())
					state->addPoints(entity.component<PointComponent>().get()->points);
				if (entity.has_component<FlightComponent>())
				{
					entity.remove<FlightComponent>();
					if (entity.has_component<Target>())
					{
						entity.remove<Target>();
					}
				}
				auto handle = entity.component<ParticleComponent>();
				if (!handle)
				{
					handle = entity.assign<ParticleComponent>();
				}
				events.emit<AddParticleEvent>(EXPLOSION, handle, 3);
			}
		}
	}
};