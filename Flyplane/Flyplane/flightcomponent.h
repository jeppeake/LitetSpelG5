#pragma once

struct FlightComponent {
	const float density = 1.2466;

	FlightComponent(float base_speed, float boost_speed, float brake_speed, float turnrate, float engine_acceleration) : base_speed(base_speed), boost_speed(boost_speed), brake_speed(brake_speed), turnrate(turnrate), engine_acceleration(engine_acceleration) {
		drift = 1.f;
	}

	void setInput(glm::vec3 input) {
		i_roll = input.x;
		i_pitch = input.y;
		i_yaw = input.z;
	}

	float yaw = 0.f, pitch = 0.f, roll = 0.f;
	float i_yaw = 0.f, i_pitch = 0.f, i_roll = 0.f;
	float m_yaw = 1.f, m_pitch = 1.f, m_roll = 1.f;
	float throttle = 0.f, airBrake = 0.f;
	float i_throttle = 0.f, i_airBrake = 0.f;
	float drift = 1.f, i_drift = 0.f;
	float engine_power = 0.f;
	float turnrate = 0.f;
	float base_speed = 0.f;
	float brake_speed = 0.f;
	float boost_speed = 0.f;
	float previous_speed = 0.f;
	float engine_acceleration = 0.f;
	float current_speed = 0.f;

	bool afterBurner = false;
	float boost = 100.f;
	bool burnOut = false;

	//Timer driftTime;
	float driftReduction = 0.f;
	//float liftCoeff = 0.f, wingArea = 0.f;
};