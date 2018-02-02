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



		ComponentHandle<AIComponent> ai;
		ComponentHandle<FlightComponent> flight;
		ComponentHandle<Transform> transform;

		for (Entity entity : es.entities_with_components(ai, flight, transform)) {
			Behaviour* b = ai->behaviours.at(0);
			for (int i = 1; i < ai->behaviours.size(); i++) {
				if (b->getPriority() < ai->behaviours.at(i)->getPriority()) {
					b = ai->behaviours.at(i);
				}
			}

			ComponentHandle<PlayerComponent> p_player;
			ComponentHandle<Transform> p_transform;
			ComponentHandle<FlightComponent> p_flight;
			for (Entity entity : es.entities_with_components(p_player, p_transform, p_flight))
			{
				glm::vec3 input = b->act(p_transform, p_flight, transform, flight, ai->is_targeted);

				//ai->prioritize();
				flight->setInput(input);
			}
		}
	};
};