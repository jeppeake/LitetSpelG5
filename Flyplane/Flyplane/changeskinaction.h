#pragma once

#include "ButtonAction.h"
#include "loadoutstate.h"

class ChangeSkinAction : public ButtonAction {
private:
	LoadoutState * menuS;
	unsigned int select;
public:
	ChangeSkinAction(LoadoutState* ms, unsigned int select) {
		menuS = ms;
		this->select = select;
	}
	inline void action() {
		menuS->changeSkin(select);
	}
};