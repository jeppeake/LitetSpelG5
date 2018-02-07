#include "aisystem.h"
void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) {

	ComponentHandle<Terrain> terrain;
	for (Entity entity_terrain : es.entities_with_components(terrain)) {
		ComponentHandle<PlayerComponent> p_player;
		ComponentHandle<Transform> p_transform;
		ComponentHandle<FlightComponent> p_flight;
		for (Entity entity_player : es.entities_with_components(p_player, p_transform, p_flight)) {
			ComponentHandle<AIComponent> ai;
			ComponentHandle<FlightComponent> flight;
			ComponentHandle<Transform> transform;
			for (Entity entity_ai : es.entities_with_components(ai, flight, transform)) {
				if (ai->behaviours.size() != 0) {
					for (int i = 0; i < ai->behaviours.size(); i++) {
						ai->behaviours.at(i)->setActive(ai->behaviours.at(i)->condition->test(entity_player, entity_ai, entity_terrain));
					}

					Behaviour* b = ai->behaviours.at(0);
					for (int i = 1; i < ai->behaviours.size(); i++) {
						if (b->getPriority() < ai->behaviours.at(i)->getPriority() && ai->behaviours.at(i)->getActive() && !ai->behaviours.at(i)->terminated) {
							b = ai->behaviours.at(i);
						}
					}
					Commands com;
					glm::vec3 input;
					if (b->getActive()) {
						com = b->act(entity_player, entity_ai, entity_terrain);
					}

					flight->setInput(com.steering);
				}
			}
		}
	}
};