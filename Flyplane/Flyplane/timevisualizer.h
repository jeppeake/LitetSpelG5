#pragma once

#include <string>

namespace TimeVisualizer {

	void init();

	void startTiming(const std::string& name);

	void stopTiming();

	void draw();

};