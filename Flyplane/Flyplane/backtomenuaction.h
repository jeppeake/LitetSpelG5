#pragma once

#include "ButtonAction.h"
#include "menustate.h"
#include "optionsstate.h"
#include "highscorestate.h"
#include "playingstate.h"

class BackToMenuAction : public ButtonAction {
private:
	OptionsState * optionS = nullptr;
	PlayingState * playingS = nullptr;
	HighscoreState * highscoreS = nullptr;
public:
	BackToMenuAction(OptionsState* os) {
		optionS = os;
	}
	BackToMenuAction(PlayingState* ps) {
		playingS = ps;
	}
	
	BackToMenuAction(HighscoreState* hs) {
		highscoreS = hs;
	}
	inline void action() {
		if (optionS != nullptr)
			optionS->startMenu();
		else if (highscoreS != nullptr)
			highscoreS->startMenu();
		else
			playingS->startMenu();
	}
};
