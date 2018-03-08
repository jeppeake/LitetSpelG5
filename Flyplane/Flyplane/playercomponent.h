#pragma once
#include "camera.h"
#include "timer.h"

struct PlayerComponent {
	PlayerComponent() {

	}
	bool flyByWire = false;

	Timer outsideTimer;
	bool isOutside = false;
	double outsideTimeLeft = 0;


	Timer flareTimer;
	bool flareActive = false;
	double flareAccum = 0;

	float coolDown = 7.f;
};