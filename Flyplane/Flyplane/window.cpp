#include "window.h"
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <string>

namespace
{
	std::unordered_map<GLFWwindow*, glm::vec2 > scrolls;

	void scrollCallback(GLFWwindow* window, double x, double y)
	{
		scrolls[window] += glm::vec2(x, y);
	}
}


Window::Window()
{}

Window::~Window()
{
	close();
}

bool Window::isInitialized()
{
	if (window)
		return true;
	else
		return false;
}
void Window::open(int width, int height)
{
	if (window)
		return;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	title = "Flyplane";


	//glfwWindowHint(GLFW_DEPTH_BITS, 16);
	//glfwWindowHint(GLFW_STENCIL_BITS, 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);


	window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "glewInit failed: " << std::string((const char*)glewGetErrorString(err)) << "\n";
		std::cin.ignore();
		exit(1);
	}

	scrolls[window] = glm::vec2(0, 0);
	glfwSetScrollCallback(window, scrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
}

void Window::open()
{
	if (window)
		return;
	if (!glfwInit())
	{
		std::cout << "failed to initiate GLFW" << std::endl;
		std::cin.ignore();
		exit(EXIT_FAILURE);
	}
	title = "Flyplane";

	auto display = glfwGetPrimaryMonitor();
	if (!display) {
		std::cerr << "No primary monitor found" << std::endl;
		std::cin.ignore();
		exit(-1);
	}
	const GLFWvidmode* mode = glfwGetVideoMode(display);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	window = glfwCreateWindow(mode->width, mode->height, title.c_str(), display, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "glewInit failed: " << std::string((const char*)glewGetErrorString(err)) << "\n";
		std::cin.ignore();
	}
	scrolls[window] = glm::vec2(0, 0);
	glfwSetScrollCallback(window, scrollCallback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Window::close()
{
	glfwTerminate();
}

GLFWwindow * Window::getGLFWWindow()
{
	return window;
}

void Window::update()
{
	if (frame_timer.elapsed() >= 0.5)
	{
		double elapsed = frame_timer.restart();
		double fps = frames / elapsed;
		std::string new_title = title + " | FPS: " + std::to_string(fps);
		glfwSetWindowTitle(window, new_title.c_str());
		frames = 0;
	}
	frames += 1.0;

	scroll = scrolls[window];
	scrolls[window] = glm::vec2(0, 0);


	last_mouse_pos = mouse_pos;
	glfwGetCursorPos(window, &mouse_pos.x, &mouse_pos.y);
	

	glfwSwapBuffers(window);
	glfwPollEvents();
	auto _size = size();
	glViewport(0, 0, (GLsizei)_size.x, (GLsizei)_size.y);
}

bool Window::shouldClose()
{
	if (!window)
		return true;
	return glfwWindowShouldClose(window);
}
bool Window::keyDown(int key)
{
	return GLFW_PRESS == glfwGetKey(window, key);
}

glm::vec2 Window::mouseMovement()
{
	glm::vec2 pos = mousePosition();
	glm::vec2 result = pos - last_mouse_pos;
	return result;
}

glm::vec2 Window::mousePosition()
{
	return glm::vec2(mouse_pos);
}

glm::vec2 Window::size()
{
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	return glm::vec2(width, height);
}