#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "aicomponent.h"
#include "flightcomponent.h"
#include <iostream>

using namespace entityx;

struct AISystem : public entityx::System<AISystem> {

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {

		ComponentHandle<AIComponent> ai;
		ComponentHandle<FlightComponent> flight;
		for (Entity entity : es.entities_with_components(ai, flight)) {
			ai = entity.component<AIComponent>();
			flight = entity.component<FlightComponent>();
			//ai->prioritize();
			//flight->setInput(ai->act());
		}
	};
};