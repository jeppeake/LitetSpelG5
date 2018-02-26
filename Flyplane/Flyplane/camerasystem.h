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

using namespace entityx;

struct CameraSystem : public System<CameraSystem> {

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override {

		ComponentHandle<CameraOnComponent> cameraOn;
		ComponentHandle<Transform> transform;
		for (Entity entity : es.entities_with_components(cameraOn, transform)) {
			ComponentHandle<Physics> physics = entity.component<Physics>();

			float speed = length(physics->velocity);
			glm::vec3 v;
			if (speed > 0.0001f) {
				v = normalize(physics->velocity);
			}

			glm::vec3 offset(0, 1, -10.5);

			Transform camTrans = cameraOn->camera.getTransform();

			double f = 0.005;
			camTrans.orientation = glm::mix(camTrans.orientation, transform->orientation, float(1.0 - glm::pow(f, 2.0*dt)));

			offset = camTrans.orientation * offset;
			camTrans.pos = transform->pos + offset;

			float baseFov = 80.f;
			float currentFov = cameraOn->camera.getFov();
			float spread = 25.f;

			float targetFov = baseFov + spread * (speed / 175.f - 1.f);

			float fFov = 0.0005;
			float newFov = glm::mix(currentFov, targetFov, float(1.0 - glm::pow(fFov, dt)));

			
			cameraOn->camera.setTransform(camTrans, newFov);
		}
	}

};