#pragma once

#include "ButtonAction.h"
#include "menustate.h"
#include "optionsstate.h"

class BackToMenuAction : public ButtonAction {
private:
	OptionsState * optionS = nullptr;
	PlayingState * playingS = nullptr;
public:
	BackToMenuAction(OptionsState* os) {
		optionS = os;
	}
	BackToMenuAction(PlayingState* ps) {
		playingS = ps;
	}
	inline void action() {
		if (optionS != nullptr)
			optionS->startMenu();
		else
			playingS->startMenu();
	}
};
