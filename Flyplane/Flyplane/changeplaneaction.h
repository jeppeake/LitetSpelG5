#pragma once

#include "ButtonAction.h"
#include "loadoutstate.h"

class ChangePlaneAction : public ButtonAction {
private:
	LoadoutState * menuS;
	unsigned int select;
public:
	ChangePlaneAction(LoadoutState* ms, unsigned int select) {
		menuS = ms;
		this->select = select;
	}
	inline void action() {
		menuS->changePlane(select);
	}
};