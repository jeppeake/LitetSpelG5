#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include "transform.h"
#include "physics.h"
#include "input.h"
#include <iostream>
#include "particles.h"
#include "dropcomponent.h"

using namespace entityx;

struct PhysicsSystem : public entityx::System<PhysicsSystem> {
	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {

		ComponentHandle<Physics> physics;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(physics, transform)) {
			physics = entity.component<Physics>();
			transform = entity.component<Transform>();
			
			calcAcceleration(physics);
			
			physics->velocity += physics->acceleration * float(dt);

			transform->pos += float(dt) * physics->velocity;
		}

		ComponentHandle<DropComponent> drop;
		for (Entity entity : es.entities_with_components(drop, transform)) {
			drop = entity.component<DropComponent>();
			transform = entity.component<Transform>();

			glm::quat rotation = glm::angleAxis(glm::pi<float>() * (float)dt, glm::vec3(0.f, 1.f, 0.f));
			transform->orientation = rotation * transform->orientation;
		}
	};


	void calcAcceleration(ComponentHandle<Physics> physics) {
		if (physics->gravity) {
			physics->acceleration = physics->g;
		}
	}
};