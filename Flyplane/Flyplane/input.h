#pragma once
#include <glm/common.hpp>
class Input
{
public:
	Input() = delete;
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static glm::vec2 mouseMov();
	static glm::vec2 mousePos();
};

