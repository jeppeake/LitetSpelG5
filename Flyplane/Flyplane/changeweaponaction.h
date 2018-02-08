#pragma once

#include "ButtonAction.h"
#include "loadoutstate.h"

class ChangeWeaponAction : public ButtonAction {
private:
	LoadoutState * menuS;
	unsigned int select;
public:
	ChangeWeaponAction(LoadoutState* ms, unsigned int select) {
		menuS = ms;
		this->select = select;
	}
	inline void action() {
		menuS->changeWeapon(select);
	}
};