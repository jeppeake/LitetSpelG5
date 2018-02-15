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
			if (health->health <= 0) {
				std::cout << "Entity got killed." << "\n";
				if (entity.has_component<PointComponent>())
					state->addPoints(entity.component<PointComponent>().get()->points);
				entity.destroy();
			}
		}
	}
};