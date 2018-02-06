#pragma once

class ButtonAction {
public:
	virtual ~ButtonAction() {}
	virtual void action() = 0;
};