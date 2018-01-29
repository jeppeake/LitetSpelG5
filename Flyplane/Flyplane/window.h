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
	glm::dvec2 mouse_pos;
public:

	Window();
	~Window();
	Window(const Window &other) = delete;
	void operator=(const Window &other) = delete;
	static Window &getWindow()
	{
		static Window i;
		return i;
	}
	bool isInitialized();
	void open(int width, int height);
	void open();
	void close();
	GLFWwindow *getGLFWWindow();
	void update();

	bool shouldClose();

	bool mouseButtonDown(int button);
	bool keyDown(int key);

	glm::vec2 mouseMovement();
	glm::vec2 mousePosition();
	glm::vec2 size();
};