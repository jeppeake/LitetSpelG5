#include "playersystem.h"

void PlayerSystem::update(EntityManager & es, EventManager & events, TimeDelta dt)
{
	ComponentHandle<Transform> transform;
	ComponentHandle<Physics> physics;
	ComponentHandle<PlayerComponent> player;
	ComponentHandle<FlightComponent> flight;
	for (Entity entity : es.entities_with_components(physics, transform, player, flight)) {
		physics = entity.component<Physics>();
		transform = entity.component<Transform>();
		player = entity.component<PlayerComponent>();
		flight = entity.component<FlightComponent>();

		auto mv = Input::mouseMov();

		double fbt = 0.05;
		double f = 0.01;
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

		flight->roll += mv.x*0.01;
		flight->pitch += mv.y*0.01;

		float roll = control(flight->roll, GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
		float pitch = control(flight->pitch, GLFW_KEY_UP, GLFW_KEY_DOWN);
		float yaw = control(flight->yaw, GLFW_KEY_D, GLFW_KEY_A);

		

		float throttle = flight->throttle;
		float new_throttle = 0.f;
		if (Input::isKeyDown(GLFW_KEY_W))
			new_throttle = glm::mix(throttle, 1.f, float(1.0 - glm::pow(fbt, dt)));


		float brake = flight->airBrake;
		float new_brake = 0.f;
		if (Input::isKeyDown(GLFW_KEY_S))
			new_brake = glm::mix(brake, 1.f, float(1.0 - glm::pow(fbt, dt)));;



		if (Input::gamepad_present()) {
			roll = glm::mix(flight->roll, Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_X), float(1.0 - glm::pow(f, dt)));
			pitch = glm::mix(flight->pitch, Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y), float(1.0 - glm::pow(f, dt)));
			new_throttle = glm::mix(throttle, ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2.f), float(1.0 - glm::pow(fbt, dt)));
			new_brake = glm::mix(brake, ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1) / 2.f), float(1.0 - glm::pow(fbt, dt)));
		}

		if (new_throttle != 0.f) {
			throttle = new_throttle;
		}
		else {
			new_throttle = glm::mix(throttle, 0.f, float(1.0 - glm::pow(fbt, dt)));
		}

		if (new_brake != 0.f) {
			brake = new_brake;
		}
		else {
			new_brake = glm::mix(brake, 0.f, float(1.0 - glm::pow(fbt, dt)));
		}

		float new_drift = 0.f;
		float d1 = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1) / 2.f);
		float d2 = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2.f);
		float driftFactor = 0.f;
		if (d1 > 0.5f && d2 > 0.5f) {
			driftFactor = d1 + d2 - 1.f;
		}
		new_drift = glm::mix(physics->drift, driftFactor, float(1.0 - glm::pow(fbt, dt)));
		//std::cout << new_drift << "\n";
		//std::cout << physics->drift << "\n";
		physics->drift = new_drift;

		flight->pitch = pitch;
		flight->roll = roll;
		flight->yaw = yaw;

		flight->throttle = throttle;
		flight->airBrake = brake;

	}
}
