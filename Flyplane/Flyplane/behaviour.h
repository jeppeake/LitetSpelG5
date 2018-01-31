#pragma once
#include <vector>
class Behaviour {
public:
	Behaviour(int priority) {
		this->priority = priority;
	}
	int getPriority() {
		return priority;
	}
private:
	int priority = 0;
};