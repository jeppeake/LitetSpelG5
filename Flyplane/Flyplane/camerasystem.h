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

			glm::vec3 v;
			if (length(physics->velocity) > 0.0001f) {
				v = normalize(physics->velocity);
			}

			glm::mat4 lookAt = glm::lookAt(transform->pos, transform->pos - transform->orientation * glm::vec3(0,0,1), glm::vec3(0, 1, 0));

			Transform camTrans;
			camTrans.orientation = glm::inverse(glm::quat_cast(lookAt));
			camTrans.pos = transform->pos;
			 
			glm::vec3 offset(0, 1, -5.5);

			offset = transform->orientation * offset;
			camTrans.pos += offset;

			cameraOn->camera.setTransform(camTrans);
		}
	}

};