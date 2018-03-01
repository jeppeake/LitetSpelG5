#include "namestate.h"
#include "loadingstate.h"
#include "input.h"
#include "window.h"


void NameState::checkForInput() {
	int offset = 'a' - 'A';
	if (Input::isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
		offset = 0;
	}
	for (int i = 0; i < 25; i++) {
		if (Input::isKeyPressed(GLFW_KEY_A + i) && name.size() < 17) {
			name += GLFW_KEY_A + i + offset;
		}
	}
	if (Input::isKeyPressed(GLFW_KEY_BACKSPACE) && name.size() > 0)
		name.pop_back();
}

void NameState::init() {
	
}

void NameState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	checkForInput();
	glm::vec2 pos;
	pos.x = Window::getWindow().size().x / 2 - 12*6;
	pos.y = Window::getWindow().size().y - 200;
	AssetLoader::getLoader().getMenutext()->drawText("Enter name:", pos, glm::vec3(1, 1, 1), 1.0);
	pos.x = Window::getWindow().size().x / 2 - 12 * name.length()/2;
	pos.y = Window::getWindow().size().y - 250;
	AssetLoader::getLoader().getText()->drawText(name, pos, glm::vec3(1, 1, 1), 0.7);
	if (Input::isKeyPressed(GLFW_KEY_ENTER) && name.size() > 0) {
		this->changeState(new LoadingState(name));
	}
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		this->changeState(new MenuState());
	}
}

void NameState::startMenu() {

}
