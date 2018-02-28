#pragma once

#include <entityx\entityx.h>
#include "lifetimecomponent.h"

class LifeTimeSystem : public entityx::System<LifeTimeSystem> {

public:
	void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
		
		std::vector<entityx::Entity> toRemove;

		entityx::ComponentHandle<LifeTimeComponent> lifetime;
		for (entityx::Entity e : es.entities_with_components(lifetime)) {
			double elapsed = lifetime->timer.elapsed();
			if (elapsed > lifetime->timeToLive) {
				toRemove.push_back(e);
			}
		}
		for (auto& e : toRemove) {
			e.destroy();
		}
	}
};