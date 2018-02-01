#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "aicomponent.h"
#include "transform.h"
#include "flightcomponent.h"
#include <iostream>

using namespace entityx;

struct AISystem : public entityx::System<AISystem> {

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {

		ComponentHandle<PlayerComponent> p_player;
		ComponentHandle<Transform> p_transform;
		ComponentHandle<FlightComponent> p_flight;
		for (Entity entity : es.entities_with_components(p_player, p_transform, p_flight)) {
			p_player = entity.component<PlayerComponent>();
			p_transform = entity.component<Transform>();
			p_flight = entity.component<FlightComponent>();
		}

		ComponentHandle<AIComponent> ai;
		ComponentHandle<FlightComponent> flight;
		ComponentHandle<Transform> transform;

		for (Entity entity : es.entities_with_components(ai, flight, transform)) {
			ai = entity.component<AIComponent>();
			flight = entity.component<FlightComponent>();
			transform = entity.component<Transform>();

			for (int i = 0; i < ai->behaviours.size(); i++) {
				flight->setInput(ai->behaviours.at(i)->act());
			}

			//ai->prioritize();
			//flight->setInput(ai->act());
		}
	};
};