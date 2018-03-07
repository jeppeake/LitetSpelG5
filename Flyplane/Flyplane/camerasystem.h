#pragma once

#include <entityx\entityx.h>
#include <glm\glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/noise.hpp>
#include "transform.h"
#include "playercomponent.h"
#include "cameraoncomponent.h"
#include "physics.h"
#include "flightcomponent.h"

using namespace entityx;

struct CameraSystem : public System<CameraSystem> {

	double gt = 0;

	void update(EntityManager &es, EventManager &events, TimeDelta dt) override;

};