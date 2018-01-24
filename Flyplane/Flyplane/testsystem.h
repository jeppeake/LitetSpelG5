#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "transform.h"
#include "physics.h"
#include <iostream>

using namespace entityx;

struct TestSystem : public entityx::System<TestSystem> {

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {

		ComponentHandle<Physics> physics;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(physics, transform)) {
			physics = entity.component<Physics>();
			transform = entity.component<Transform>();

			physics->calcVelocity();
			physics->calcAcceleration();
			
			transform->pos += float(dt) * physics->velocity;
		}
	};
};