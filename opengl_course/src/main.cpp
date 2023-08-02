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


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float mixVal = 0.5f;

glm::mat4 mouseTransform = glm::mat4(1.0f);
Joystick mainJ(0);

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

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn Open GL", NULL, NULL);

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
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	Shader shader("assets/vertex_core.glsl", "assets/fragment_core.glsl");
	Shader shader2("assets/vertex_core.glsl", "assets/fragment_core2.glsl");


	// vertex array.
	// NDC: Normalized Device Coordinates
	float vertices[] = {
		// positions			colors				texture
		-0.5f, -0.5f, 0.0f,	    1.0f, 1.0f, 0.5f,   0.0f, 0.0f, 	// bottom left
		-0.5f,  0.5f, 0.0f,	    0.5f, 1.0f, 0.75f,	0.0f, 1.0f,		// top left
		 0.5f, -0.5f, 0.0f,	    0.6f, 1.0f, 0.2f,	1.0f, 0.0f,		// bottom right
		 0.5f,  0.5f, 0.0f,	    1.0f, 0.2f, 1.0f,	1.0f, 1.0f,		// top right
	};

	unsigned int indices[] = {
		0, 1, 2, 
		3, 1, 2
	};

	// VAO, VBO.

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO;
	glBindVertexArray(VAO);

	// bind VBO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// set attribute pointer.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// setup ebo.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coordinates.
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// Generating Textures.
	unsigned int texture1, texture2;

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

	unsigned char* data = stbi_load("assets/chungus.png", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(
			GL_TEXTURE_2D
		);

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	data = stbi_load("assets/bg_texture.jpg", &width, &height, &nChannels, 0);

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
	shader.setInt("texture2", 1);

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.activate();
	shader.setMat4("transform", trans);

	shader2.activate();

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
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.activate();

		// rendering commands.
		glBindVertexArray(VAO);

		// drawing shapes.
		glUseProgram(shader.id);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		shader.activate();

		shader.setFloat("mixVal", mixVal);

		// send new frame to window.
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();

	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Reset everytime we reize the window.
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
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

	mainJ.update();
}

