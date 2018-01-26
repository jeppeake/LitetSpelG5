#pragma once
#include "timer.h"

struct Projectile {
	Projectile(double lifetime) : lifetime(lifetime) {}

	Timer timer;
	double lifetime;
};