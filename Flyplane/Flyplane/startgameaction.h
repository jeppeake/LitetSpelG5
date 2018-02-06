#pragma once
#include "ButtonAction.h"
#include "menustate.h"

class StartGameAction : public ButtonAction {
private:
	MenuState * menuS;
public:
	StartGameAction(MenuState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->startGame();
	}
};