#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/models/cube.hpp"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/camera.h"
#include "io/screen.h"


void processInput(double dt);

float mixVal = 0.5f;
float fov = 50.0f;

glm::mat4 mouseTransform = glm::mat4(1.0f);
Joystick mainJ(0);

glm::mat4 transform = glm::mat4(1.0f);

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float x, y, z;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera cameras[2] = {
	Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	Camera(glm::vec3(10.0f, 10.0f, 10.0f))
};

int activeCam = 0;

Screen screen;

int main()
{
	glfwInit();
	// Define the open gl version. ours is 4.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__

	if (!screen.init())
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();

	Shader shader("assets/object.vs", "assets/object.fs");

	Cube cube(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	cube.init();



	mainJ.update();

	if (mainJ.isPresent())
	{
		std::cout << mainJ.getName() << " is present." << std::endl;
	}
	else
	{
		std::cout << "Not present." << std::endl;
	}

	while (!screen.shouldClose())
	{
		double currTime = glfwGetTime();
		deltaTime = (double) currTime - lastFrame;
		lastFrame = (double) currTime;

		processInput(deltaTime);

		screen.update();

		shader.activate();

		// create transformation coordinaties
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = cameras[activeCam].getViewMatrix();
		projection = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		shader.activate();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		cube.render(shader);

		// send new frame to window.
		screen.newFrame();
	}

	glfwTerminate();

	return 0;
}


void processInput(double dt)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		screen.setShouldClose(true);
	}

	// moving cameras[activeCam].
	if (Keyboard::key(GLFW_KEY_W))
	{
		cameras[activeCam].updateCameraPos(CameraDirection::FORWARD, dt);
		
	}

	if (Keyboard::key(GLFW_KEY_S))
	{
		cameras[activeCam].updateCameraPos(CameraDirection::BACKWARD, dt);
	}

	if (Keyboard::key(GLFW_KEY_A))
	{
		cameras[activeCam].updateCameraPos(CameraDirection::RIGHT, dt);
	}

	if (Keyboard::key(GLFW_KEY_D))
	{
		cameras[activeCam].updateCameraPos(CameraDirection::LEFT, dt);
	}


	if (Keyboard::key(GLFW_KEY_SPACE))
	{
		cameras[activeCam].updateCameraPos(CameraDirection::DOWN, dt);
	}

	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
	{
		cameras[activeCam].updateCameraPos(CameraDirection::UP, dt);
	}

	double dx = Mouse::getDx(), dy = Mouse::getDy();


	if (dx != 0 || dy != 0)
	{
		cameras[activeCam].updateCameraDirection(dx, dy);
	}

	double scrollDy = Mouse::getScrollDy();

	if (scrollDy != 0)
	{
		cameras[activeCam].updateCameraZoom(scrollDy);
	}

	if (Keyboard::keyDown(GLFW_KEY_TAB))
	{
		activeCam += (activeCam == 0) ? 1 : -1;
	}

}

