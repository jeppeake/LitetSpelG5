#include "aisystem.h"
void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) {

	ComponentHandle<Terrain> terrain;
	for (Entity entity_terrain : es.entities_with_components(terrain)) {
		ComponentHandle<PlayerComponent> p_player;
		ComponentHandle<Transform> p_transform;
		ComponentHandle<FlightComponent> p_flight;
		for (Entity entity_player : es.entities_with_components(p_player, p_transform, p_flight)) {
			ComponentHandle<AIComponent> ai_ai;
			ComponentHandle<FlightComponent> ai_flight;
			ComponentHandle<Transform> ai_transform;
			for (Entity entity_ai : es.entities_with_components(ai_ai, ai_flight, ai_transform)) {

				Entity entity_closest;
				float closest = 1000000000.f;
				float new_closest = 0.f;

				ComponentHandle<FlightComponent> closest_flight;
				ComponentHandle<Transform> closest_transform;
				ComponentHandle<ModelComponent> closest_model;

				for (Entity entity_closest_search : es.entities_with_components(closest_flight, closest_transform)) {
					new_closest = glm::length(closest_transform->pos - ai_transform->pos);
					//std::cout << new_closest << "\n";
					if (new_closest < closest && new_closest > 1.f) {
						entity_closest = entity_closest_search;
						closest = new_closest;
					}
				}

				//std::cout << closest << "\n";
				if (ai_ai->behaviours.size() != 0) {
					for (int i = 0; i < ai_ai->behaviours.size(); i++) {
						ai_ai->behaviours.at(i)->setActive(ai_ai->behaviours.at(i)->condition->test(entity_player, entity_ai, entity_terrain, entity_closest));
					}

					Behaviour* b = ai_ai->behaviours.at(0);
					for (int i = 1; i < ai_ai->behaviours.size(); i++) {
						if (b->getPriority() < ai_ai->behaviours.at(i)->getPriority() && ai_ai->behaviours.at(i)->getActive() && !ai_ai->behaviours.at(i)->terminated) {
							b = ai_ai->behaviours.at(i);
						}
					}
					Commands com;
					glm::vec3 input;
					if (b->getActive()) {
						com = b->act(entity_player, entity_ai, entity_terrain, entity_closest);
					}

					ai_flight->setInput(com.steering);
					if (ai_ai->throttle_allowed) {
						ai_flight->i_throttle = com.throttle;
					}
					if (ai_ai->brake_allowed) {
						ai_flight->i_airBrake = com.brake;
					}
					if (com.brake > 0.f) {
						//std::cout << "BRAKING\n";
					}
					if (com.throttle > 0.f) {
						//std::cout << "BOOSTING\n";
					}
					if (entity_ai.has_component<Equipment>()) {
						entity_ai.component<Equipment>()->primary.at(0).shouldFire = com.fire_primary;
						if (com.fire_primary) {
							//std::cout << "FIRING!" << std::endl;
						}
					}
				}
			}
		}
	}
};