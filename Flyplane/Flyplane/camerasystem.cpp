#include "camerasystem.h"

#include "input.h"
#include "modelcomponent.h"
#include <GLFW\glfw3.h>



void CameraSystem::update(EntityManager & es, EventManager & events, TimeDelta dt) {

	/*if (Input::isKeyPressed(GLFW_KEY_U)) {
		following = !following;
	}*/

	if (following) {
		gt += dt;

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

			glm::vec3 offset(0, 1, -4.5);

			auto model = entity.component<ModelComponent>();
			if (model) {
				offset = 2.5f*model->mptr->getBoundingRadius()*normalize(offset);
			}

			Transform camTrans = cameraOn->camera.getTransform();

			cam = &(cameraOn->camera);

			double fOrientation = 0.0005;
			camTrans.orientation = glm::mix(camTrans.orientation, transform->orientation, float(1.0 - glm::pow(fOrientation, 2.0*dt)));


			cameraOn->shake = glm::clamp(cameraOn->shake, 0.f, 1.f);
			//std::cout << "Shake: " << cameraOn->shake << "\n";

			float shakeI = pow(cameraOn->shake, 3);

			float offsetSpeed = 15.f;

			glm::vec3 maxOffset(1);
			glm::vec3 shakeOffset;
			shakeOffset.x = shakeI * glm::perlin(glm::vec3(offsetSpeed * gt, 0, 0));
			shakeOffset.y = shakeI * glm::perlin(glm::vec3(0, offsetSpeed * gt, 0));
			//shakeOffset.z = cameraOn->shake*glm::perlin(glm::vec3(0, 0, gt));
			offset += maxOffset * shakeOffset;

			float maxAngle = 0.09;

			float angleSpeed = 15.f;
			float pitch = maxAngle * shakeI * glm::perlin(glm::vec3(20, 20, angleSpeed * gt));
			float yaw = maxAngle * shakeI * glm::perlin(glm::vec3(angleSpeed * gt, 20, 20));
			float roll = maxAngle * shakeI * glm::perlin(glm::vec3(20, angleSpeed * gt, 20));

			camTrans.orientation = glm::quat(glm::vec3(pitch, yaw, roll))* camTrans.orientation;

			offset = camTrans.orientation * offset;

			if (physics) {
				camTrans.pos += physics->velocity * float(dt);
			}

			double fPos = 0.07;
			glm::vec3 targetPos = transform->pos + offset * (0.5f*glm::smoothstep(brakeSpeed, boostSpeed, speed) + 1);
			camTrans.pos = glm::mix(camTrans.pos, targetPos, float(1.0 - glm::pow(fPos, 2.0*dt)));

			float baseFov = 85.f;
			float currentFov = cameraOn->camera.getFov();
			float spread = 10.f;

			float targetFov = baseFov;
			targetFov += spread * glm::smoothstep(baseSpeed, boostSpeed, speed);
			targetFov -= spread * glm::smoothstep(baseSpeed, brakeSpeed, speed);
			targetFov += 2.f*spread * glm::smoothstep(boostSpeed, 2*boostSpeed, speed);

			float fFov = 0.0002;
			float newFov = glm::mix(currentFov, targetFov, float(1.0 - glm::pow(fFov, dt)));


			cameraOn->camera.setTransform(camTrans, newFov);

			cameraOn->shake = glm::mix(cameraOn->shake, 0.f, float(1.0 - glm::pow(0.005, dt)));

			/*
			if (Input::isKeyDown(GLFW_KEY_H)) {
			cameraOn->shake += 1.5f*dt;
			}
			*/
		}
	
	}
	else {
		//free camera
		if (Input::isKeyDown(GLFW_KEY_I)) {
			glm::vec3 newPos = cam->getTransform().pos + cam->getTransform().orientation * glm::vec3(0,0,1);
			Transform trans(newPos);
			cam->setTransform(trans);
		}
	}
}
