#pragma once
#include "camera.h"

struct PlayerComponent {
	PlayerComponent() {

	}
	bool flyByWire = false;
	Camera camera;
};