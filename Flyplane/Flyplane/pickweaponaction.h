#pragma once

#include "ButtonAction.h"
#include "loadoutstate.h"

class PickWeaponAction : public ButtonAction {
private:
	LoadoutState * menuS;
	unsigned int select;
public:
	PickWeaponAction(LoadoutState* ms, unsigned int select) {
		menuS = ms;
		this->select = select;
	}
	inline void action() {
		menuS->pickWeapon(select);
	}
};