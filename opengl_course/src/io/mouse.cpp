#include "mouse.h"

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::scrollX = 0;
double Mouse::scrollY = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::cursorPosCallback(GLFWwindow* window, double _x, double _y) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	// normalize coordinates
	x = (2 * _x / width) - 1;
	y = 2 * (1 - (2 * _y / width));

	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	dx = x - lastX;
	dy = lastY - y;
	firstMouse = false;
	lastX = x;
	lastY = y;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	if (action != GLFW_RELEASE)
	{
		buttons[button] = true;
	}
	else
	{
		buttons[button] = false;
	}

	buttonsChanged[button] = action != GLFW_RELEASE;
}

double Mouse::getMouseX() {
	return x;
}

double Mouse::getMouseY() {
	return y;
}

bool Mouse::button(int _button) {
	return buttons[_button];
}

void Mouse::mouseWheelCallback(GLFWwindow* window, double dx, double dy)
{
	scrollX = dx;
	scrollY = dy;
}

double Mouse::getDx()
{
	double _dx = dx;
	dx = 0;
	return _dx;
}
double Mouse::getDy()
{
	double _dy = dy;
	dy = 0;
	return _dy;
}

double Mouse::getScrollDx()
{
	double dx = scrollX;
	scrollX = 0;
	return dx;
}
double Mouse::getScrollDy()
{
	double dy = scrollY;
	scrollY = 0;
	return dy;
}

bool Mouse::buttonChanged(int button)
{
	bool ret = buttonsChanged[button];
	buttonsChanged[button] = false;
	return ret;
}
bool Mouse::buttonUp(int button)
{
	return !buttons[button] && buttonChanged(button);
}
bool Mouse::buttonDown(int button)
{
	return buttons[button] && buttonChanged(button);
}

