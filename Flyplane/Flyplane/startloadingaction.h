#pragma once
#include "ButtonAction.h"
#include "menustate.h"
#include "loadingstate.h"

class StartLoadingAction : public ButtonAction {
private:
	MenuState * menuS;
public:
	StartLoadingAction(MenuState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->startGame();
	}
};