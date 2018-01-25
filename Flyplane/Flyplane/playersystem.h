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


			Transform cam = *transform.get();
			glm::vec3 offset(2, 3, -10);
			offset = glm::toMat3(cam.orientation)*offset;
			cam.pos += offset;
			player->camera.setTransform(cam);

			

			//std::cout << Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X) << " ";
			//std::cout << Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_Y) << "\n";

			glm::vec3 angular_vel;

			angular_vel.z = physics->turnRate * 1.5f * Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X);
			angular_vel.x = -physics->turnRate * Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y);

			
			angular_vel = glm::toMat3(cam.orientation)*angular_vel;

			/*
			float x = (glm::toMat3(cam.orientation) * glm::vec3(0.0, 0.0, 1.0)).x * float(glm::sin(physics->turnRate * Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_X) / 2));
			float y = (glm::toMat3(cam.orientation) * glm::vec3(0.0, 0.0, 1.0)).y * float(glm::sin(physics->turnRate / 2));
			float z = (glm::toMat3(cam.orientation) * glm::vec3(0.0, 0.0, 1.0)).z * float(glm::sin(physics->turnRate / 2));
			float w = cos(physics->turnRate / 2);

			transform->orientation = glm::quat(x, y, z, w);
			*/

			glm::quat avq{
				0,
				angular_vel.x,
				angular_vel.y,
				angular_vel.z
			};
			glm::quat spin = 0.5f*avq*transform->orientation;
			transform->orientation += spin * float(dt);
			transform->orientation = normalize(transform->orientation);


			physics->velocity = glm::toMat3(cam.orientation)*glm::vec3(0.0, 0.0, 1.0) * (((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2) * (100 * float(dt)) + 50);
		}
	}

};