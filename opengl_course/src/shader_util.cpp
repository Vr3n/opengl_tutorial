#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include "shader_util.h"

std::string loadShaderSrc(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "Could not open " << filename << std::endl;
	}

	file.close();

	return ret;
}

void compileShader(unsigned int &shader_name, const char* shader_file_path, GLenum shaderType)
{
	int success;
	char infoLog[512];

	shader_name = glCreateShader(shaderType);
	std::string vertShaderSrc = loadShaderSrc(shader_file_path);
	const GLchar* vertShader = vertShaderSrc.c_str();
	glShaderSource(shader_name, 1, &vertShader, NULL);
	glCompileShader(shader_name);

	// Catch error.
	glGetShaderiv(shader_name, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader_name, 512, NULL, infoLog);
		std::cout << "Error with vertex shader compilation: " << std::endl << infoLog << std::endl;
	}
}
