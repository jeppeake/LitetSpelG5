#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "transform.h"
#include "physics.h"
#include "input.h"
#include <iostream>

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
		//std::cout << Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X) << " ";
		//std::cout << Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_Y) << "\n";
		//std::cout << Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) << "\n";
		//std::cout << Input::gamepad_button_pressed(GLFW_GAMEPAD_BUTTON_CROSS) << "\n";
	};


	void calcAcceleration(ComponentHandle<Physics> physics) {
		//physics->acceleration = physics->g;
	}
};