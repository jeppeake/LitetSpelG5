#pragma once


#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include "renderer.h"
#include "modelcomponent.h"
#include "transform.h"

#include "physics.h"
#include "playercomponent.h"
#include "flightcomponent.h"

#include "input.h "

using namespace entityx;

struct PlayerSystem : public System<PlayerSystem> {

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {
		ComponentHandle<Transform> transform;
		ComponentHandle<Physics> physics;
		ComponentHandle<PlayerComponent> player;
		ComponentHandle<FlightComponent> flight;
		for (Entity entity : es.entities_with_components(physics, transform, player, flight)) {
			physics = entity.component<Physics>();
			transform = entity.component<Transform>();
			player = entity.component<PlayerComponent>();
			flight = entity.component<FlightComponent>();

			flight->pitch = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y);
			flight->roll = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X);

			flight->throttle = (Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1);
			flight->airBrake = (Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1);
		}
	}

};