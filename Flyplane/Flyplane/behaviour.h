#pragma once

class Behaviour {
public:
	Behaviour(int priority) {
		this->priority = priority;
	}
	virtual int getPriority() {
		return priority;
	}

	virtual glm::vec3 act() = 0; //add player, friendlies, (hostile) missiles and aircraft orientation to function call

	int priority = 0;
};