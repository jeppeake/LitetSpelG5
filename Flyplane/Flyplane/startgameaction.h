#pragma once
#include "ButtonAction.h"
#include "menustate.h"
#include "loadingstate.h"

class StartGameAction : public ButtonAction {
private:
	 LoadingState * menuS;
public:
	StartGameAction(LoadingState* ms) {
		menuS = ms;
	}
	inline void action() {
		menuS->startGame();
	}
};