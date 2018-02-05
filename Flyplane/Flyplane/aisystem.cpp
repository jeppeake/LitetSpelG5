#include "aisystem.h"
void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) {

	ComponentHandle<PlayerComponent> p_player;
	ComponentHandle<Transform> p_transform;
	ComponentHandle<FlightComponent> p_flight;
	for (Entity entity : es.entities_with_components(p_player, p_transform, p_flight)) {
		ComponentHandle<AIComponent> ai;
		ComponentHandle<FlightComponent> flight;
		ComponentHandle<Transform> transform;

		for (Entity entity : es.entities_with_components(ai, flight, transform)) {

			Behaviour* b = ai->behaviours.at(0);

			for (int i = 1; i < ai->behaviours.size(); i++) {
				if (b->getPriority() < ai->behaviours.at(i)->getPriority() && ai->behaviours.at(i)->active) {
					b = ai->behaviours.at(i);
				}
			}

			glm::vec3 input;
			if (b->active) {
				input = b->act(p_transform, p_flight, transform, flight, ai->is_targeted);
			}
			else {
				input = glm::vec3(0.0, 0.0, 0.0);
			}

			flight->setInput(input);
		}
	}
};