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


		auto control = [](float init, int key1, int key2, float dt) {
			double f = 0.001;
			float val = init;
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

		float roll = control(flight->roll, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, dt);
		float pitch = control(flight->pitch, GLFW_KEY_UP, GLFW_KEY_DOWN, dt);
		//float yaw = control(flight->pitch, GLFW_KEY_UP, GLFW_KEY_DOWN, dt);



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
