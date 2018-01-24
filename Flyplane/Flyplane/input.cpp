#include "input.h"
#include "window.h"
#include <unordered_map>

std::unordered_map<int, bool> keys;
bool Input::isKeyDown(int key)
{
	if (!Window::getWindow().isInitialized())
		return false;
	int state = glfwGetKey(Window::getWindow().getGLFWWindow(), key);
	if (state == GLFW_PRESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::isKeyPressed(int key)
{
	/*
		Fix dis shit
	*/
	if (!Window::getWindow().isInitialized())
		return false;
	int state = glfwGetKey(Window::getWindow().getGLFWWindow(), key);
	if (state == GLFW_RELEASE && keys[key])
	{
		keys[key] = false;
		return true;
	}
	else if(state == GLFW_PRESS && !keys[key])
	{
		return false;
	}
	else
	{
		keys[key] = false;
		return false;
	}
}

glm::vec2 Input::mouseMov()
{
	return Window::getWindow().mouseMovement();
}

glm::vec2 Input::mousePos()
{
	return Window::getWindow().mousePosition();
}
