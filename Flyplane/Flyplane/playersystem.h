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


			float roll = 0.0;
			if (Input::isKeyDown(GLFW_KEY_LEFT))
				roll -= 1.0;
			if (Input::isKeyDown(GLFW_KEY_RIGHT))
				roll += 1.0;

			float pitch = 0.0;
			if (Input::isKeyDown(GLFW_KEY_UP))
				pitch -= 1.0;
			if (Input::isKeyDown(GLFW_KEY_DOWN))
				pitch += 1.0;


			float throttle = 0.0;
			if (Input::isKeyDown(GLFW_KEY_W))
				throttle = 1.0;

			float brake = 0.0;
			if (Input::isKeyDown(GLFW_KEY_S))
				brake = 1.0;



			if (Input::gamepad_present()) {
				roll = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_X);
				pitch = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y);
				throttle = (Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2.f;
				brake = (Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1) / 2.f;
			}

			flight->pitch = pitch;
			flight->roll = roll;

			flight->throttle = throttle;
			flight->airBrake = brake;


			
		}
	}

};