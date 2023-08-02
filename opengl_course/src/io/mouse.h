#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse {
public:
	static void cursorPosCallback(GLFWwindow* window, double _x, double _y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseWheelCallback(GLFWwindow* window, double dx, double dy);

	static double getMouseX();
	static double getMouseY();

	static double getDx();
	static double getDy();


	static double getScrollDx();
	static double getScrollDy();

	static bool button(int _button);
	static bool buttonChanged(int button);
	static bool buttonUp(int button);
	static bool buttonDown(int button);

private:
	static double x;
	static double y;
	static double dx;
	static double dy;

	static double lastX;
	static double lastY;

	static double scrollX;
	static double scrollY;

	static bool firstMouse;
	static bool buttons[];
	static bool buttonsChanged[];
};