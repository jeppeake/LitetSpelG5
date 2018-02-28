#pragma once

#include "timer.h"

struct LifeTimeComponent {
	LifeTimeComponent(double timeToLive): timeToLive(timeToLive) {}
	double timeToLive;
	Timer timer;
};