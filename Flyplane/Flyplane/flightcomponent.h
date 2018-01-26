#pragma once
#include "weapon.h"

struct FlightComponent {
	FlightComponent(float jaw, float pitch, float roll, float throttle, float airBrake) : jaw(jaw), pitch(pitch), roll(roll), throttle(throttle), airBrake(airBrake) {}
	float jaw, pitch, roll;
	float throttle, airBrake;
};