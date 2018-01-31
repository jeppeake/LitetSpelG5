#pragma once

class Behaviour {
public:
	Behaviour(int priority) {
		this->priority = priority;
	}
	virtual int getPriority() {
		return priority;
	}

	virtual glm::vec3 act() = 0;

	int priority = 0;
};