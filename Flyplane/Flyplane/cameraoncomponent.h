#pragma once
#include "camera.h"
struct CameraOnComponent {
	CameraOnComponent() {}

	float shake = 0;
	Camera camera;
};