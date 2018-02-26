#include "globaltimer.h"

namespace {
	Timer globalTimer;
	double elapsedTime = 0.0;
}

void GlobalTimer::update() {
	elapsedTime = globalTimer.elapsed();
}

double GlobalTimer::elapsed() {
	return elapsedTime;
}
