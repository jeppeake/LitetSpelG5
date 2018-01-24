#include "input.h"
#include "window.h"
#include <unordered_map>
#include <iostream>
namespace
{
	static bool inititalized = false;
	static std::unordered_map<int, int> keys;
	static std::unordered_map<int, int> pre_keys;
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			pre_keys[key]++;
		}
	}
}
bool Input::initialize()
{
	if (!Window::getWindow().isInitialized() || inititalized)
	{
		return false;
	}
	glfwSetKeyCallback(Window::getWindow().getGLFWWindow(), key_callback);
	inititalized = true;
	return true;
}
bool Input::isKeyDown(int key)
{
	return Window::getWindow().keyDown(key);
}

bool Input::isKeyPressed(int key)
{
	if (keys[key] > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Input::reset()
{
	keys = pre_keys;
	pre_keys.clear();
}

glm::vec2 Input::mouseMov()
{
	return Window::getWindow().mouseMovement();
}

glm::vec2 Input::mousePos()
{
	return Window::getWindow().mousePosition();
}
