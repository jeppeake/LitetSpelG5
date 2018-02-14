#pragma once
#include "timer.h"
#include "targetcomponent.h"

struct Projectile {
	Projectile(double lifetime, unsigned int parentFaction) : lifetime(lifetime), parentFaction(parentFaction) {}
	Projectile(double lifetime, unsigned int parentFaction, double damage) : lifetime(lifetime), parentFaction(parentFaction), damage(damage) {}
	Timer timer;
	double lifetime;
	unsigned int parentFaction = FACTION_PLAYER;
	double damage = 10;
};