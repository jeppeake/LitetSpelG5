#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include "timer.h"

class Window
{
	GLFWwindow* window = nullptr;
	glm::vec2 scroll;
	Timer frame_timer;
	double frames;
	std::string title;
	glm::vec2 last_mouse_pos;
public:

	Window();
	~Window();

	void open(int width, int height);
	void open();
	void close();

	void update();

	bool shouldClose();

	bool keyDown(int key);

	glm::vec2 mouseMovement();
	glm::vec2 mousePosition();
	glm::vec2 size();
};