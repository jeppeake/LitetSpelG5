#pragma once

#include "ButtonAction.h"
#include "playingstate.h"

class RestartAction : public ButtonAction {
private:
	PlayingState * playingS;
public:
	RestartAction(PlayingState* ps) {
		playingS = ps;
	}
	inline void action() {
		playingS->restart();
	}
};