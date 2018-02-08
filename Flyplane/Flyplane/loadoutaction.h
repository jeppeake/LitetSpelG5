#pragma once

#include "ButtonAction.h"
#include "menustate.h"

class LoadoutAction : public ButtonAction {
private:
	MenuState * menuS;
public:
	LoadoutAction(MenuState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->startLoadout();
	}
};