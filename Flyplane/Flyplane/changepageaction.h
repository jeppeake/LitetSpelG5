#pragma once

#include "ButtonAction.h"
#include "loadoutstate.h"

class ChangePageAction : public ButtonAction {
private:
	LoadoutState * menuS;
	int select;
public:
	ChangePageAction(LoadoutState* ms, int select) {
		menuS = ms;
		this->select = select;
	}
	inline void action() {
		menuS->changePage(select);
	}
};
