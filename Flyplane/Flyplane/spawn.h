#pragma once

#include <entityx\entityx.h>

struct RotateComponent {
	RotateComponent(float speed = 1) : speed(speed) {}
	float speed ;
};

namespace Spawn {

	void landmarks(entityx::EntityManager & es);

}