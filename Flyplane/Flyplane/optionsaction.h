#pragma once

#include "ButtonAction.h"
#include "menustate.h"

class OptionsAction : public ButtonAction {
private:
	MenuState * menuS;
public:
	OptionsAction(MenuState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->startOptions();
	}
};