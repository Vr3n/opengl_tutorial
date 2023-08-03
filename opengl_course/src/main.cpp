#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "graphics/shader.h"
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


	// vertex array.
	// NDC: Normalized Device Coordinates
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// VAO, VBO.

	unsigned int VAO, VBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// bind VAO;
	glBindVertexArray(VAO);

	// bind VBO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// set attribute pointer.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// texture coordinates.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	// Generating Textures.
	unsigned int texture1;

	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// load img.
	int width, height, nChannels;

	unsigned char* data = stbi_load("assets/bg_texture.jfif", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(
			GL_TEXTURE_2D
		);

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);


	shader.activate();
	shader.setInt("texture1", 0);

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// create transformation coordinaties
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, (float) (glfwGetTime() / 10.0) * glm::radians(-55.0f), glm::vec3(0.5f));
		view = cameras[activeCam].getViewMatrix();
		projection = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		shader.activate();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// rendering commands.
		glBindVertexArray(VAO);
		shader.activate();

		// drawing shapes.
		glUseProgram(shader.id);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// send new frame to window.
		screen.newFrame();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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

