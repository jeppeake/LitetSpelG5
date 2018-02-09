#pragma once

#include "ButtonAction.h"
#include "loadoutstate.h"

class SaveLoadoutAction : public ButtonAction {
private:
	LoadoutState * menuS;
public:
	SaveLoadoutAction(LoadoutState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->saveLoadout();
	}
};