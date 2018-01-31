#pragma once

#include <entityx\entityx.h>
#include <entityx\Entity.h>
#include "transform.h"
#include "physics.h"
#include "input.h"
#include "flightcomponent.h"
#include <iostream>
#include <glm/gtx/projection.hpp>

using namespace entityx;

struct FlightSystem : public entityx::System<FlightSystem> {

	void update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) override {

		ComponentHandle<Physics> physics;
		ComponentHandle<Transform> transform;
		ComponentHandle<FlightComponent> flight;
		for (Entity entity : es.entities_with_components(physics, transform, flight)) {
			physics = entity.component<Physics>();
			transform = entity.component<Transform>();
			flight = entity.component<FlightComponent>();

			auto control = [dt](float init, int key1, int key2) {
				double f = 0.01;
				float val = glm::clamp(init, -1.0f, 1.0f);
				float new_val = 0.f;
				if (Input::isKeyDown(key1))
					new_val = glm::mix(val, -1.f, float(1.0 - glm::pow(f, dt)));
				if (Input::isKeyDown(key2))
					new_val = glm::mix(val, 1.f, float(1.0 - glm::pow(f, dt)));
				if (new_val != 0.f)
					val = new_val;
				else
					val = glm::mix(val, 0.f, float(1.0 - glm::pow(f, dt)));
				return val;
			};

			float f = 0.01;
			flight->roll = glm::mix(flight->roll, flight->i_roll, float(1.0 - glm::pow(f, dt)));
			flight->pitch = glm::mix(flight->pitch, flight->i_pitch, float(1.0 - glm::pow(f, dt)));
			flight->yaw = glm::mix(flight->yaw, flight->i_yaw, float(1.0 - glm::pow(f, dt)));

			float fbt = 0.05;
			flight->i_throttle = glm::mix(flight->throttle, flight->i_throttle, float(1.0 - glm::pow(fbt, dt)));
			flight->i_airBrake = glm::mix(flight->airBrake, flight->i_airBrake, float(1.0 - glm::pow(fbt, dt)));

			flight->drift = glm::mix(flight->drift, flight->i_drift, float(1.0 - glm::pow(fbt, dt)));

			if (flight->i_throttle != 0.f) {
				flight->throttle = flight->i_throttle;
			}
			else {
				flight->throttle = glm::mix(flight->throttle, 0.f, float(1.0 - glm::pow(fbt, dt)));
			}

			if (flight->i_airBrake != 0.f) {
				flight->airBrake = flight->i_airBrake;
			}
			else {
				flight->airBrake = glm::mix(flight->airBrake, 0.f, float(1.0 - glm::pow(fbt, dt)));
			}

			glm::vec3 angular_vel;

			angular_vel.z = flight->turnrate * flight->roll;
			angular_vel.x = -flight->turnrate * flight->pitch;
			angular_vel.y = flight->turnrate * flight->yaw;

			angular_vel = glm::toMat3(transform->orientation)*angular_vel;

			glm::quat avq{
				0,
				angular_vel.x,
				angular_vel.y,
				angular_vel.z
			};
			glm::quat spin = 0.5f*avq*transform->orientation;
			transform->orientation += spin * float(dt);
			transform->orientation = normalize(transform->orientation);

			float boost = 100;
			float breakForce = 100;
			float normalSpeed = 100;
			float boost = 200;
			float breakForce = 100;
			float normalSpeed = 200;

			float maxDrift = 0.002;

			float drift = glm::pow(flight->drift, 0.005);
			float driftFactor = (1.00000000001f + maxDrift- drift);

			float speed = (normalSpeed + (boost * flight->throttle) - (breakForce * flight->airBrake));
			physics->velocity = glm::toMat3(transform->orientation) * glm::vec3(0.0, 0.0, 1.0) * speed * driftFactor + physics->velocity * (drift - (drift * maxDrift));
		}
	};
};