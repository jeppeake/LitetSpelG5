#pragma once
#include "glm/gtx/projection.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include <entityx\entityx.h>
#include <entityx\Entity.h>

#include "transform.h"
#include "flightcomponent.h"
#include "physics.h"
#include "equipment.h"

#include <iostream>
namespace SAIB {//STANDARD ARTIFICIAL INTELLIGENCE BEHAVIOURS

	glm::vec3 flyTo(glm::vec3 position, glm::quat orientation, glm::vec3 target);
	glm::vec3 flyToRes(glm::vec3 position, glm::quat orientation, glm::vec3 target, glm::vec3 restrictions);
	glm::vec3 fly_to(glm::vec3 position, glm::quat orientation, glm::vec3 target);

	float rollTowards(glm::vec3 position, glm::quat orientation, glm::vec3 target);

	float pitchTowards(glm::vec3 position, glm::quat orientation, glm::vec3 target);

	float yawTowards(glm::vec3 position, glm::quat orientation, glm::vec3 target);

	float testAxis(glm::vec3 position, glm::quat orientation, glm::vec3 target, glm::vec3 axis);
	float test2Axis(glm::vec3 position, glm::quat orientation, glm::vec3 target, glm::vec3 rot_axis, glm::vec3 test_axis);

	float testTowards(glm::vec3 v1, glm::vec3 v2, glm::vec3 t);

	glm::vec3 calculateInterdiction(entityx::Entity target, entityx::Entity interdictor);
}