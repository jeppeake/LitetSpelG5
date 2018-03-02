#pragma once

#include "ButtonAction.h"
#include "loadoutstate.h"

class OpenPreorderAction : public ButtonAction {
private:
	LoadoutState * menuS;
	int select;
public:
	OpenPreorderAction(LoadoutState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->startPreorder();
	}
};
