#include "flightsystem.h"

FlightSystem::FlightSystem()
{

}

void FlightSystem::update(entityx::EntityManager &es, entityx::EventManager &events, TimeDelta dt) {
	ComponentHandle<Physics> physics;
	ComponentHandle<Transform> transform;
	ComponentHandle<FlightComponent> flight;
	for (Entity entity : es.entities_with_components(physics, transform, flight)) {
		physics = entity.component<Physics>();
		transform = entity.component<Transform>();
		flight = entity.component<FlightComponent>();

		auto control = [dt](float init, int key1, int key2)
		{
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

		float fd = 0.0001;
		flight->drift = glm::mix(flight->drift, flight->i_drift, float(1.0 - glm::pow(fd, dt)));
		//std::cout << "D: " << flight->drift << "\n";

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

		float maxDrift = 0.015;

		float drift = glm::pow(flight->drift, dt * 0.05);
		float driftFactor = (1.00000000001f + maxDrift - drift);

		if (driftFactor < 0.02f) {
			flight->driftReduction += float(dt) * 0.05;
			flight->driftReduction = glm::clamp(flight->driftReduction, 0.f, 0.7f);
		}
		else {
			flight->driftReduction -= float(dt) * 0.1;
			flight->driftReduction = glm::clamp(flight->driftReduction, 0.f, 0.7f);
		}

		float boostSpeed = flight->boost_speed;
		float breakSpeed = flight->brake_speed;
		float normalSpeed = flight->base_speed;
		float acc = flight->engine_acceleration * 0.05f;
		float currentSpeed = flight->current_speed;

		float boostFactor = glm::clamp(flight->throttle - flight->airBrake, 0.f, 1.f);
		float brakeFactor = glm::clamp(flight->airBrake - flight->throttle, 0.f, 1.f);

		float speedAim;
		if (!flight->afterBurner || flight->burnOut) {
			speedAim = normalSpeed + ((boostSpeed - normalSpeed) * boostFactor) + ((breakSpeed - normalSpeed) * brakeFactor);//speed components
			flight->boost += dt * 10.f;
			if (flight->burnOut) {
				flight->throttle = 0.f;
			}
			if (flight->boost >= 100.f && flight->burnOut) {
				flight->burnOut = false;
			}
		}
		else if (!flight->burnOut){
			speedAim = boostSpeed * 2.f * flight->throttle;
			flight->boost -= dt * 10.f;
			//std::cout << flight->boost << "\n";
			if (flight->boost < 10.f) {
				flight->throttle = flight->boost / 10.f;
			}
			if (flight->boost <= 0.f) {
				flight->afterBurner = false;
				flight->burnOut = true;
			}
			//play sound?
			auto cameraOn = entity.component<CameraOnComponent>();
			if (cameraOn) {
				cameraOn->shake += 2.f * flight->throttle * dt;
			}
		}
		flight->boost = glm::clamp(flight->boost, 0.f, 100.f);

		float speed = glm::mix(speedAim, currentSpeed, float(1.0 - glm::pow(acc, dt))) * (1.f - flight->driftReduction);

		glm::vec3 angular_vel;
		float turnrateAim = (flight->turnrate + (flight->turnrate * flight->drift * 2.f) - (flight->turnrate * flight->throttle / 2) + (flight->turnrate * flight->airBrake / 2)) * (1.f - flight->driftReduction);
		float turnrateACC = 0.005;
		float turnrate = glm::mix(turnrateAim, flight->turnrate, float(1.0 - glm::pow(turnrateACC, dt)));

		angular_vel.z = turnrate * 1.5 * flight->roll * flight->m_roll;
		angular_vel.x = -turnrate * flight->pitch* flight->m_pitch;
		angular_vel.y = turnrate * 0.5 * flight->yaw* flight->m_yaw;

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

		glm::vec3 normalFlight = glm::toMat3(transform->orientation) * glm::vec3(0.0, 0.0, 1.0) * speed * driftFactor;
		glm::vec3 driftFlight = physics->velocity * (drift - (drift * maxDrift));

		physics->velocity = normalFlight + driftFlight;
		//physics->velocity = glm::toMat3(transform->orientation) * glm::vec3(0.0, 0.0, 1.0) * speed * driftFactor + physics->velocity * (drift - (drift * maxDrift));

		float minSpeed = breakSpeed;
		float gravFactor = (1.f - (glm::clamp(glm::length(physics->velocity), minSpeed, 0.f) / minSpeed)) * dt;

		physics->velocity += physics->g * gravFactor;

		//std::cout << gravFactor << "\n";
		flight->current_speed = glm::length(physics->velocity);
		/*
		glm::vec3 dir;
		if (Input::isKeyDown(GLFW_KEY_A))
			dir.x += 1;
		if (Input::isKeyDown(GLFW_KEY_D))
			dir.x -= 1;
		if (Input::isKeyDown(GLFW_KEY_W))
			dir.z += 1;
		if (Input::isKeyDown(GLFW_KEY_S))
			dir.z -= 1;
		if (Input::isKeyDown(GLFW_KEY_SPACE))
			dir.y += 1;
		if (Input::isKeyDown(GLFW_KEY_LEFT_CONTROL))
			dir.y -= 1;

		speed = 500;
		if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT))
			speed = 3000;

		physics->velocity = glm::toMat3(transform->orientation) * dir * speed;
		*/
	}
}