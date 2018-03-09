#include "camerasystem.h"

#include "input.h"
#include <GLFW\glfw3.h>

void CameraSystem::update(EntityManager & es, EventManager & events, TimeDelta dt) {

	if (Input::isKeyPressed(GLFW_KEY_U)) {
		following = !following;
	}

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

			Transform camTrans = cameraOn->camera.getTransform();

			cam = &(cameraOn->camera);

			double fOrientation = 0.0005;
			camTrans.orientation = glm::mix(camTrans.orientation, transform->orientation, float(1.0 - glm::pow(fOrientation, 2.0*dt)));


			float maxShake = 8.f;
			cameraOn->shake = glm::min(maxShake, cameraOn->shake);

			//std::cout << "Shake: " << cameraOn->shake << "\n";

			glm::vec3 shakeOffset;
			shakeOffset.x = 0.02f*cameraOn->shake*glm::perlin(glm::vec3(5 * gt, 0, 0));
			shakeOffset.y = 0.02f*cameraOn->shake*glm::perlin(glm::vec3(0, 5 * gt, 0));
			//shakeOffset.z = cameraOn->shake*glm::perlin(glm::vec3(0, 0, gt));
			offset += shakeOffset;

			glm::vec3 axis;
			axis.x = glm::perlin(glm::vec3(5 * gt, 20, 20));
			axis.y = glm::perlin(glm::vec3(20, 5 * gt, 20));
			axis.z = glm::perlin(glm::vec3(20, 20, 5 * gt)) + 0.1;
			axis = normalize(axis);

			float angle = 0.05f*cameraOn->shake*glm::perlin(glm::vec3(-20, -20, 20 * gt));

			camTrans.orientation = glm::rotate(glm::quat(), angle, axis) * camTrans.orientation;

			offset = camTrans.orientation * offset;

			if (physics) {
				camTrans.pos += physics->velocity * float(dt);
			}

			double fPos = 0.01;
			glm::vec3 targetPos = transform->pos + offset * (0.5f*glm::smoothstep(brakeSpeed, boostSpeed, speed) + 1);
			camTrans.pos = glm::mix(camTrans.pos, targetPos, float(1.0 - glm::pow(fPos, 2.0*dt)));

			float baseFov = 85.f;
			float currentFov = cameraOn->camera.getFov();
			float spread = 10.f;

			float targetFov = baseFov;
			targetFov += spread * glm::smoothstep(baseSpeed, boostSpeed, speed);
			targetFov -= spread * glm::smoothstep(baseSpeed, brakeSpeed, speed);

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
