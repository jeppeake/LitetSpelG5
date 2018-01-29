#pragma once

struct FlightComponent {
	const float density = 1.2466;

	FlightComponent(float engine_power, float turnrate) : engine_power(engine_power), turnrate(turnrate) {}

	float calcLift(float velocity) {
		return liftCoeff * ((density * glm::pow(velocity, 2)) / 2) * wingArea;
	}

	float yaw = 0.f, pitch = 0.f, roll = 0.f;
	float throttle = 0.f, airBrake = 0.f;
	float engine_power = 0.f;
	float turnrate = 0.f;
	float liftCoeff = 0.f, wingArea = 0.f;
};