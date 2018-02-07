#pragma once

#include "ButtonAction.h"
#include "menustate.h"

class HighscoreAction : public ButtonAction {
private:
	MenuState * menuS;
public:
	HighscoreAction(MenuState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->startHighscore();
	}
};