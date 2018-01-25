#pragma once


#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include "renderer.h"
#include "modelcomponent.h"
#include "transform.h"

#include "physics.h"
#include "playercomponent.h"

#include "input.h "

using namespace entityx;

struct PlayerSystem : public System<PlayerSystem> {

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		ComponentHandle<Transform> transform;
		ComponentHandle<Physics> physics;
		ComponentHandle<PlayerComponent> player;
		for (Entity entity : es.entities_with_components(physics, transform, player)) {
			physics = entity.component<Physics>();
			transform = entity.component<Transform>();
			player = entity.component<PlayerComponent>();
			//std::cout << Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X) << " ";
			//std::cout << Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_Y) << "\n";
			physics->velocity = glm::vec3(Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X), Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_Y), 0.0);
			transform->orientation += glm::quat(dt * Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y),-dt * Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y), -dt * Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_X), dt * Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_X));
			transform->orientation = normalize(transform->orientation);
		}
	}

};