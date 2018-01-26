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


		double f = 0.001;

		float roll = flight->roll;

		float new_roll = 0.f;
		if (Input::isKeyDown(GLFW_KEY_LEFT))
			new_roll = glm::mix(roll, -1.f, float(1.0 - glm::pow(f, dt)));
		if (Input::isKeyDown(GLFW_KEY_RIGHT))
			new_roll = glm::mix(roll, 1.f, float(1.0 - glm::pow(f, dt)));
		if (new_roll != 0.f)
			roll = new_roll;
		else
			roll = glm::mix(roll, 0.f, float(1.0 - glm::pow(f, dt)));

		

		float pitch = flight->pitch;
		float new_pitch = 0.f;
		if (Input::isKeyDown(GLFW_KEY_UP))
			new_pitch = glm::mix(pitch, -1.f, float(1.0 - glm::pow(f, dt)));
		if (Input::isKeyDown(GLFW_KEY_DOWN))
			new_pitch = glm::mix(pitch, 1.f, float(1.0 - glm::pow(f, dt)));
		if (new_pitch != 0.f)
			pitch = new_pitch;
		else
			pitch = glm::mix(pitch, 0.f, float(1.0 - glm::pow(f, dt)));


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
