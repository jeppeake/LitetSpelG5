#pragma once

#include <entityx\entityx.h>
#include <glm\glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "transform.h"
#include "playercomponent.h"
#include "cameraoncomponent.h"
#include "physics.h"
#include "flightcomponent.h"

using namespace entityx;

struct CameraSystem : public System<CameraSystem> {

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {

		ComponentHandle<CameraOnComponent> cameraOn;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(cameraOn, transform)) {
			ComponentHandle<Physics> physics = entity.component<Physics>();

			float speed = 0;
			glm::vec3 v;
			if (physics) {
				speed = length(physics->velocity);
				if (speed > 0.0001f) {
					v = normalize(physics->velocity);
				}
			}

			float boostSpeed = 250.f;
			float baseSpeed = 175.f;
			float brakeSpeed = 100.f;
			auto flight = entity.component<FlightComponent>();
			if (flight) {
				boostSpeed = flight->boost_speed;
				baseSpeed = flight->base_speed;
				brakeSpeed = flight->brake_speed;
			}

			glm::vec3 offset(0, 1, -10.5);

			Transform camTrans = cameraOn->camera.getTransform();

			double fOrientation = 0.0005;
			camTrans.orientation = glm::mix(camTrans.orientation, transform->orientation, float(1.0 - glm::pow(fOrientation, 2.0*dt)));

			offset = camTrans.orientation * offset;
			camTrans.pos = transform->pos + offset * (0.5f*glm::smoothstep(brakeSpeed, boostSpeed, speed)+1);

			float baseFov = 90.f;
			float currentFov = cameraOn->camera.getFov();
			float spread = 10.f;

			float targetFov = baseFov;
			targetFov += spread * glm::smoothstep(baseSpeed, boostSpeed, speed);
			targetFov -= spread * glm::smoothstep(baseSpeed, brakeSpeed, speed);

			float fFov = 0.0005;
			float newFov = glm::mix(currentFov, targetFov, float(1.0 - glm::pow(fFov, dt)));

			
			cameraOn->camera.setTransform(camTrans, newFov);
		}
	}

};