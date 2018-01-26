#pragma once
#include "weapon.h"

struct FlightComponent {
	FlightComponent(float engine_power, float turnrate) : engine_power(engine_power), turnrate(turnrate) {}
	float jaw, pitch, roll = 0.f;
	float throttle, airBrake = 0.f;
	float engine_power;
	float turnrate;
};