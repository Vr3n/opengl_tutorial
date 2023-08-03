#include "screen.h"	
#include "keyboard.h"
#include "mouse.h"

unsigned int Screen::SCR_WIDTH = 1280;
unsigned int Screen::SCR_HEIGHT = 720;

void Screen::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

Screen::Screen() : window(nullptr) {}

bool Screen::init()
{
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learning Opengl", NULL, NULL);

	if (!window)
	{
		return false;
	}
	glfwMakeContextCurrent(window);
	return true;
}


void Screen::setParameters()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

	glEnable(GL_DEPTH_TEST);

	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
}


void Screen::update()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::newFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Screen::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose)
{
	glfwSetWindowShouldClose(window, shouldClose);
}