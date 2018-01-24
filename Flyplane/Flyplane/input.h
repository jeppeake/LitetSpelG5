#pragma once
#include <glm/common.hpp>
class Input
{
public:
	Input() = delete;
	static bool initialize();
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static void reset();
	static glm::vec2 mouseMov();
	static glm::vec2 mousePos();
};

