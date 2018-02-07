#include "buttonhandler.h"
#include "window.h"
#include "playingstate.h"
#include "ButtonAction.h"

void ButtonHandler::handleButtonClicks() {
	for (auto button : buttons) {
		glm::vec2 cp = glm::vec2(Window::getWindow().mousePosition().x, Window::getWindow().size().y - Window::getWindow().mousePosition().y);
		glm::vec2 dpos = glm::vec2(button->pos.x, Window::getWindow().size().y - button->pos.y - 48.f);
		
		if (cp.x > dpos.x && cp.x < (dpos.x + button->dimensions.x) && cp.y > dpos.y && cp.y < (dpos.y + button->dimensions.y)) {
			button->dcolor = button->hcolor;
			if (!button->playedSound) {
				button->soundHover.play();
				button->playedSound = true;
			}
			if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				button->action->action();
				button->soundClick.play();
			}
		}
		else {
			button->dcolor = button->color;
			button->playedSound = false;
		}
			
	}
}