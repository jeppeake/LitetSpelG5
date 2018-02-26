#pragma once
#include "model.h"
#include <entityx\entityx.h>
#include "timer.h"

struct WeaponStats {
	//projectile model, sound, range?, ammo, speed, 
	WeaponStats(unsigned int ammo, unsigned int lifetime, float speed, float mass, float cooldown, bool infAmmo) : ammo(ammo), lifetime(lifetime), speed(speed), mass(mass), cooldown(cooldown), infAmmo(infAmmo) {}
	WeaponStats(unsigned int ammo, unsigned int lifetime, float speed, float mass, float cooldown, bool infAmmo, double damage) : ammo(ammo), lifetime(lifetime), speed(speed), mass(mass), cooldown(cooldown), infAmmo(infAmmo), damage(damage) {}
	WeaponStats(unsigned int ammo, unsigned int lifetime, float speed, float mass, float cooldown, bool infAmmo, float turnRate, float detonateRange, float explodeRadius, float damage, float droptime = 1) : ammo(ammo), lifetime(lifetime), speed(speed), mass(mass), cooldown(cooldown), infAmmo(infAmmo), turnRate(turnRate),
	detonateRange(detonateRange), explodeRadius(explodeRadius), damage(damage), droptime(droptime) {}
	unsigned int ammo, lifetime;
	float speed, mass, cooldown, damage = 10;
	float turnRate, detonateRange, explodeRadius, droptime; //missile stuff
	bool infAmmo;
};

struct Weapon {
	Weapon(WeaponStats stats, Model* model, Model* projectileModel, glm::vec3 offset) : stats(stats), model(model), projectileModel(projectileModel), offset(offset) { }
	Weapon(WeaponStats stats, Model* model, Model* projectileModel, glm::vec3 offset, glm::vec3 scale, glm::vec3 projScale, glm::quat rot) : stats(stats), model(model), projectileModel(projectileModel), offset(offset), scale(scale), projScale(projScale), rot(rot) { }
	Weapon(WeaponStats stats, Model* model, Model* projectileModel, glm::vec3 offset, glm::vec3 scale, glm::vec3 projScale, glm::quat rot, bool isMissile, bool dissappear) : stats(stats), model(model), projectileModel(projectileModel), offset(offset), scale(scale), projScale(projScale), rot(rot), isMissile(isMissile), dissappear(dissappear) { }
	WeaponStats stats;
	Model* model;
	Model* projectileModel;
	bool shouldFire = false;
	bool active = false;
	bool isMissile = false;
	bool dissappear = false;
	Timer timer;
	glm::vec3 offset = glm::vec3(1.0,-0.5,0.0);
	glm::vec3 projScale = glm::vec3(1);
	glm::vec3 scale = glm::vec3(1);
	glm::quat rot = glm::quat();
	unsigned int group = 0;
};

struct ExplosionComponent {
	ExplosionComponent(double damage, double radius) : damage(damage), radius(radius) {}
	double damage;
	double radius;
};