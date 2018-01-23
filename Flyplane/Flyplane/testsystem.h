#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "transform.h"
#include <iostream>

using namespace entityx;

struct TestSystem : public entityx::System<TestSystem> {

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {


		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(transform)) {
			transform = entity.component<Transform>();
			transform->pos += glm::vec3(0.1);

			std::cout << transform->pos.x << "\n";
		}
	};
};