#pragma once

struct Target {
	Target() {};
	Target(double heat) : heat(heat) {}
	double heat = 10;
};