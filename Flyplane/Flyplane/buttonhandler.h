#pragma once
#include "button.h"
#include <vector>

class ButtonHandler {
private:
	
public:
	std::vector<Button*> buttons;
	ButtonHandler() {}
	~ButtonHandler() {}

	void addButton(Button* button) {
		buttons.push_back(button);
	}

	void drawButtons() {
		for(auto button : buttons) {
			button->drawButton();
		}
	}

	void clearButtons() {
		buttons.clear();
	}

	void handleButtonClicks();
};