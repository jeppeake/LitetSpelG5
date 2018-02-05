#pragma once
#include "timer.h"
#include "targetcomponent.h"

struct Projectile {
	Projectile(double lifetime, unsigned int parentFaction) : lifetime(lifetime), parentFaction(parentFaction) {}

	Timer timer;
	double lifetime;
	unsigned int parentFaction = FACTION_PLAYER;
};