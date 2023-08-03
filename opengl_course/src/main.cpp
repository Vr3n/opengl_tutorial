#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "shader.h"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/camera.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double dt);

float mixVal = 0.5f;
float fov = 50.0f;

glm::mat4 mouseTransform = glm::mat4(1.0f);
Joystick mainJ(0);

glm::mat4 transform = glm::mat4(1.0f);

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float x, y, z;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Learn Open GL", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set Initial viewport size.
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");


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

	
	x = 0.0f;
	y = 0.0f;
	z = 3.0f;

	mainJ.update();

	if (mainJ.isPresent())
	{
		std::cout << mainJ.getName() << " is present." << std::endl;
	}
	else
	{
		std::cout << "Not present." << std::endl;
	}

	while (!glfwWindowShouldClose(window))
	{
		double currTime = glfwGetTime();
		deltaTime = (double) currTime - lastFrame;
		lastFrame = (double) currTime;

		processInput(window, deltaTime);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// create transformation coordinaties
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, (float) (glfwGetTime() / 10.0) * glm::radians(-55.0f), glm::vec3(0.5f));
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

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
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Reset everytime we reize the window.
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void processInput(GLFWwindow* window, double dt)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}

	// change the mix value.
	if (Keyboard::keyDown(GLFW_KEY_UP))
	{	
		mixVal += 0.05f;
		if (mixVal > 1)
		{
			mixVal = 1.0f;
		}
	}

	if (Keyboard::keyDown(GLFW_KEY_DOWN))
	{	
		mixVal -= 0.05f;
		if (mixVal < 0)
		{
			mixVal = 0.0f;
		}
	}

	// moving camera.
	if (Keyboard::key(GLFW_KEY_W))
	{
		camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}

	if (Keyboard::key(GLFW_KEY_S))
	{
		camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}

	if (Keyboard::key(GLFW_KEY_A))
	{
		camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}

	if (Keyboard::key(GLFW_KEY_D))
	{
		camera.updateCameraPos(CameraDirection::LEFT, dt);
	}


	if (Keyboard::key(GLFW_KEY_SPACE))
	{
		camera.updateCameraPos(CameraDirection::DOWN, dt);
	}

	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
	{
		camera.updateCameraPos(CameraDirection::UP, dt);
	}

	double dx = Mouse::getDx(), dy = Mouse::getDy();

	std::cout << "dx: " << dx << std::endl;
	std::cout << "dy: " << dy << std::endl;

	if (dx != 0 || dy != 0)
	{
		camera.updateCameraDirection(dx, dy);
	}

	double scrollDy = Mouse::getScrollDy();

	if (scrollDy != 0)
	{
		camera.updateCameraZoom(scrollDy);
	}

}

