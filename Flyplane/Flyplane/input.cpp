#include "input.h"
#include "window.h"
#include <unordered_map>
#include <iostream>
namespace
{
	static bool inititalized = false;
	static std::unordered_map<int, int> keys;
	static std::unordered_map<int, int> pre_keys;

	float axis_threshold = 0.2;

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

bool Input::gamepad_present() {
	int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
	if (present) {
		std::cout << "Joystick detected" << "\n";
		if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
		{
			std::cout << "Gamepad mode" << "\n";
			return true;
		}
	}
	return false;
}

float Input::gamepad_axis(int axis) {
	GLFWgamepadstate gamepad_state;
	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad_state)){
		float ret = gamepad_state.axes[axis];
		if ((ret > 0 && ret < axis_threshold) || (ret < 0 && ret > -axis_threshold)) {
			ret = 0.f;
		}
		return ret;
	}
}

bool Input::gamepad_button_pressed(int button) {
	GLFWgamepadstate gamepad_state;
	if (glfwGetGamepadState(GLFW_JOYSTICK_1, &gamepad_state)) {
		return gamepad_state.buttons[GLFW_GAMEPAD_BUTTON_A];
	}
}
