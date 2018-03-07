#pragma once
#include "model.h"
#include <entityx\entityx.h>
#include "weapon.h"
struct TurretPlacement {
	TurretPlacement(glm::quat orientation, glm::vec3 scale, glm::vec3 offset, glm::vec3 front) : orientation(orientation), scale(scale), offset(offset), front(front) {}
	glm::vec3 scale = glm::vec3(1.f);
	glm::vec3 offset = glm::vec3();
	glm::quat orientation = glm::quat(1.f, 0.f, 0.f, 0.f);
	glm::vec3 front = glm::vec3();
};

struct TurretInfo {
	TurretInfo(float turnrate, glm::vec2 elevationLimit, glm::vec2 traverseLimit, float range, Model* turret, Model* gun) : turnrate(turnrate), elevationLimit(elevationLimit), traverseLimit(traverseLimit), range(range), turret(turret), gun(gun) {}
	float turnrate = 0.f;
	float range = 0.f;
	glm::vec2 elevationLimit = glm::vec2();
	glm::vec2 traverseLimit = glm::vec2();
	glm::vec2 ET = glm::vec2(0.f, 0.f);
	Model* turret;
	Model* gun;
};

struct WeaponInfo {
	WeaponInfo(glm::vec3 projScale, Model* projModel, glm::vec3 projectileOffset) : projScale(projScale), projModel(projModel), projectileOffset(projectileOffset){}
	glm::vec3 projScale = glm::vec3(1.f);
	Model* projModel;
	glm::vec3 projectileOffset = glm::vec3(1.f);
};

struct Turret {
	Turret(WeaponStats stats, TurretInfo info, TurretPlacement placement, WeaponInfo weapon, bool autoFire) : stats(stats), info(info), placement(placement), weapon(weapon), autoFire(autoFire) {}
	WeaponStats stats;
	TurretInfo info;
	TurretPlacement placement;
	WeaponInfo weapon;
	Timer timer;
	double timeAccum = 0;
	bool shouldFire = false;
	bool autoFire = false;

	glm::quat getMountOrientation() {
		return placement.orientation * glm::angleAxis(info.ET.x, glm::vec3(0.f, 1.f, 0.f));
	}

	glm::quat getGunOrientation() {
		return placement.orientation * glm::angleAxis(info.ET.x, glm::vec3(0.f, 1.f, 0.f)) * glm::angleAxis(info.ET.y, glm::vec3(1.f, 0.f, 0.f));
	}
};