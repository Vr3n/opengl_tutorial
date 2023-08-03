#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
	unsigned int id;

	Shader();
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	void activate();

	void generate(const char* vertexShaderPath, const char* fragShaderPath);

	// utility functions.
	std::string loadShaderSrc(const char* filepath);
	GLuint compileShader(const char* filepath, GLenum shaderType);

	// uniform functions.
	void setMat4(const std::string& name, glm::mat4 val);
	void setInt(const std::string& name, int val);
	void setBool(const std::string& name, bool val);
	void setFloat(const std::string& name, float val);
	void set4Float(const std::string& name, float val1, float val2, float val3, float val4);
};

