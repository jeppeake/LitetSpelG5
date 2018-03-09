#pragma once
#include "camera.h"
#include "timer.h"
#include <entityx\Entity.h>

struct PlayerComponent {
	PlayerComponent() {

	}
	bool flyByWire = false;

	Timer outsideTimer;
	bool isOutside = false;
	double outsideTimeLeft = 0;


	const float coolDown = 7.f;
	double flareTime = coolDown;
	bool flareActive = false;
	double flareAccum = 0;


	bool hasTarget = false;
	entityx::Entity target;
};