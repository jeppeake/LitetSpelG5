#pragma once
#include <glm\glm.hpp>
#include "text.hpp"
#include "ButtonAction.h"

class Button {
private:
	
public:
	std::string bText = "Button Text";
	glm::vec2 pos = glm::vec2(500, 500);
	glm::vec2 dimensions = glm::vec2(150, 50);
	glm::vec3 color = glm::vec3(0, 255, 0);
	glm::vec3 hcolor = glm::vec3(255, 0, 0);
	glm::vec3 dcolor = glm::vec3(255, 0, 0);
	ButtonAction* action;
	Button() {}
	Button(std::string bText, glm::vec2 pos, glm::vec2 dimensions, glm::vec3 color, glm::vec3 hcolor, ButtonAction* action) {
		this->bText = bText;
		this->color = color;
		this->hcolor = hcolor;
		this->pos = pos;
		this->dimensions = dimensions;
		this->action = action;
	}
	Button(std::string bText, glm::vec2 pos, glm::vec2 dimensions, glm::vec3 color, glm::vec3 hcolor) {
		this->bText = bText;
		this->color = color;
		this->hcolor = hcolor;
		this->pos = pos;
		this->dimensions = dimensions;
	}
	~Button() {}
	void drawButton();
};