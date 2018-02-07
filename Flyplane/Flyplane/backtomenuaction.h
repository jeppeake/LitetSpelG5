#pragma once

#include "ButtonAction.h"
#include "menustate.h"
#include "optionsstate.h"
#include "loadoutstate.h"
#include "playingstate.h"

class BackToMenuAction : public ButtonAction {
private:
	OptionsState * optionS = nullptr;
	PlayingState * playingS = nullptr;
	LoadoutState * loadoutS = nullptr;
public:
	BackToMenuAction(OptionsState* os) {
		optionS = os;
	}
	BackToMenuAction(PlayingState* ps) {
		playingS = ps;
	}
	BackToMenuAction(LoadoutState* ls) {
		loadoutS = ls;
	}
	inline void action() {
		if (optionS != nullptr)
			optionS->startMenu();
		else if (loadoutS != nullptr) {
			loadoutS->startMenu();
		}
		else
			playingS->startMenu();
	}
};
