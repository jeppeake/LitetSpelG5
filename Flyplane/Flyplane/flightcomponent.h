#pragma once

struct FlightComponent {
	const float density = 1.2466;

	FlightComponent(float engine_power, float turnrate) : engine_power(engine_power), turnrate(turnrate) {}

	float calcLift(float velocity) {
		return liftCoeff * ((density * glm::pow(velocity, 2)) / 2) * wingArea;
	}

	float jaw, pitch, roll = 0.f;
	float throttle, airBrake = 0.f;
	float engine_power;
	float turnrate;
	float liftCoeff, wingArea;
};