#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "shader_util.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	int success;
	char infoLog[512];

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


	/*
		Shaders.
	*/

	// Compile Vertex Shader.
	unsigned int vertexShader;
	compileShader(vertexShader, "assets/vertex_core.glsl", GL_VERTEX_SHADER);

	// Compile fragment Shader.
	unsigned int fragmentShader;
	compileShader(fragmentShader, "assets/fragment_core.glsl", GL_FRAGMENT_SHADER);

	// Compile fragment Shader 2.
	unsigned int fragmentShader2;
	compileShader(fragmentShader2, "assets/fragment_core2.glsl", GL_FRAGMENT_SHADER);


	unsigned int shaderPrograms[2];
	shaderPrograms[0] = glCreateProgram();

	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShader);
	glLinkProgram(shaderPrograms[0]);

	// Catch error.
	glGetProgramiv(shaderPrograms[0], GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderPrograms[0], 512, NULL, infoLog);
		std::cout << "Error with shader program: " << std::endl << infoLog << std::endl;
	}

	shaderPrograms[1] = glCreateProgram();

	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShader);
	glLinkProgram(shaderPrograms[1]);

	// Catch error.
	glGetProgramiv(shaderPrograms[1], GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shaderPrograms[0], 512, NULL, infoLog);
		std::cout << "Error with shader program: " << std::endl << infoLog << std::endl;
	}

	// deleting the shaders as we have linked them.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	// vertex array.
	// NDC: Normalized Device Coordinates
	float vertices[] = {
		// positions			colors
		-0.25f, -0.5f, 0.0f,	1.0f, 1.0f, 0.5f,	// bottom left
		0.15f, 0.0f, 0.0f,	0.5f, 1.0f, 0.75f,	// top left
		0.0f, 0.5f, 0.0f,	0.6f, 1.0f, 0.2f,	// bottom right
		0.5f, -0.4f, 0.0f,	1.0f, 0.2f, 1.0f	// top right
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// setup ebo.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glUseProgram(shaderPrograms[0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);
		
		trans = glm::rotate(
			trans,
			glm::radians((float)glfwGetTime() / 100.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		glUniformMatrix4fv(glGetUniformLocation(shaderPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(trans));


		// rendering commands.
		glBindVertexArray(VAO);

		// drawing shapes.
		glUseProgram(shaderPrograms[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glUseProgram(shaderPrograms[1]);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(13 * sizeof(unsigned int)));


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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

