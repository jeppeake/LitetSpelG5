#pragma once

struct FlightComponent {
	const float density = 1.2466;

	FlightComponent(float base_speed, float turnrate) : base_speed(base_speed), turnrate(turnrate) {}

	void setInput(glm::vec3 input) {
		i_roll = input.x;
		i_pitch = input.y;
		i_yaw = input.z;
	}

	float yaw = 0.f, pitch = 0.f, roll = 0.f;
	float i_yaw = 0.f, i_pitch = 0.f, i_roll = 0.f;
	float throttle = 0.f, airBrake = 0.f;
	float i_throttle = 0.f, i_airBrake = 0.f;
	float drift = 0.f, i_drift = 0.f;
	float engine_power = 0.f;
	float turnrate = 0.f;
	float base_speed = 0.f;
	float current_speed = 0.f;
	//float liftCoeff = 0.f, wingArea = 0.f;
};