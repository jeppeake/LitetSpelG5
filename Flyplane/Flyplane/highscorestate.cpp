#include "highscorestate.h"
#include "backtomenuaction.h"
#include "window.h"

void HighscoreState::init() {
	bHandler.addButton(new Button("Back to menu", glm::vec2(100, 100), glm::vec2(220, 36), glm::vec3(1, 1, 1), glm::vec3(0.5, 0.5, 0.5), new BackToMenuAction(this), "buttonback"));
}

void HighscoreState::update(double dt) {
	glClearColor(33.0 / 255, 33.0 / 255, 33.0 / 255, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	AssetLoader::getLoader().getBigtext()->drawText("Highscore", glm::vec2(100, Window::getWindow().size().y - 100), glm::vec3(1, 1, 1), 1);
	bHandler.drawButtons();
	bHandler.handleButtonClicks();

	glm::vec2 pos;
	pos.x = 470;
	pos.y = 630;
	AssetLoader::getLoader().getHighscoreText()->drawText("HIGH SCORES", pos, glm::vec3(1, 1, 1), 1.5);
	pos.x = 400;
	pos.y = 600;
	
	for (int i = 0; i < 9; i++) {
		pos.y -= 50;
		AssetLoader::getLoader().getHighscoreText()->drawText(highscore[i], pos, glm::vec3(1, 1, 1), 1.0);
		AssetLoader::getLoader().getHighscoreText()->drawText(" " + to_string(i + 1) + ". ", pos - glm::vec2(60, 0), glm::vec3(1, 1, 1), 1);
	}
	pos.y -= 50;
	AssetLoader::getLoader().getHighscoreText()->drawText(highscore[9], pos, glm::vec3(1, 1, 1), 1.0);
	AssetLoader::getLoader().getHighscoreText()->drawText(to_string(10) + ". ", pos - glm::vec2(60, 0), glm::vec3(1, 1, 1), 1);
}

void HighscoreState::startMenu() {
	this->changeState(new MenuState());
}
