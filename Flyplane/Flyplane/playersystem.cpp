#include "playersystem.h"
PlayerSystem::PlayerSystem()
{
	
}
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

		auto control1 = [dt](int key1) {
			if (Input::isKeyDown(key1)) {
				return 1.f;
			}
			return 0.f;
		};
		auto control2 = [dt](int key1, int key2) {
			if (Input::isKeyDown(key1)) {
				return -1.f;
			}
			else if (Input::isKeyDown(key2)) {
				return 1.f;
			}
			return 0.f;
		};

		flight->roll += mv.x * 0.005;
		flight->roll = glm::clamp(flight->roll, -1.f, 1.f);
		flight->pitch += mv.y * 0.005;
		flight->pitch = glm::clamp(flight->pitch, -1.f, 1.f);

		float roll = control2(GLFW_KEY_LEFT, GLFW_KEY_RIGHT);
		float pitch = control2(GLFW_KEY_UP, GLFW_KEY_DOWN);
		float yaw = control2(GLFW_KEY_D, GLFW_KEY_A);

		float throttle = control1(GLFW_KEY_W);
		float brake = control1(GLFW_KEY_S);

		if (Input::gamepad_present()) {
			roll = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_X);
			pitch = Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_Y);
			yaw = -Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_X);
			throttle = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2.f);
			brake = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1) / 2.f);
		}

		float new_drift = 0.f;
		float d1 = 0;
		float d2 = 0;

		if (Input::gamepad_present()) {
			d1 = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1) / 2.f);
			d2 = ((Input::gamepad_axis(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1) / 2.f);
		}
		float driftFactor = 0.f;
		if (d1 > 0.5f && d2 > 0.5f) {
			driftFactor = d1 + d2 - 1.f;
		}
		if (Input::isKeyDown(GLFW_KEY_SPACE)) {
			driftFactor = 1.0f;
		}

		flight->i_drift = driftFactor;

		if (player->flyByWire) {
			glm::vec3 ABS_up = glm::vec3(0.f, 1.f, 0.f);
			glm::vec3 front = transform->orientation * glm::vec3(0.f, 0.f, 1.f);
			glm::vec3 fixed_left = glm::cross(ABS_up, front);
			glm::vec3 fixed_up = glm::cross(fixed_left, front);
			glm::vec3 flyToPos = transform->pos + (fixed_left * yaw) + (fixed_up * -pitch) + front;
			glm::vec3 input = SAIB::flyTo(transform->pos, transform->orientation, flyToPos);
			flight->setInput(input);
			std::cout << roll << std::endl;
			if (roll > 0.1f || roll < -0.1f) {
				flight->i_roll = roll;
			}
		}
		else {

			flight->i_pitch = pitch;
			flight->i_roll = roll;
			flight->i_yaw = yaw;
		}

		flight->i_throttle = throttle;
		flight->i_airBrake = brake;
	}
}
