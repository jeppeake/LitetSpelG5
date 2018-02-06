#pragma once
#include "button.h"
#include <vector>

class ButtonHandler {
private:
	std::vector<Button*> buttons;
public:
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

	void handleButtonClicks();
};