#include "shader.h"
#include <vector>

Shader::Shader () {}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::generate(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int success;
	char infoLog[512];

	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader Linking error: " << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

}

GLuint Shader::compileShader(const char* filepath, GLenum shaderType)
{
	int success;
	char infoLog[512];

	GLuint shader_name = glCreateShader(shaderType);
	std::string vertShaderSrc = loadShaderSrc(filepath);
	const GLchar* vertShader = vertShaderSrc.c_str();
	glShaderSource(shader_name, 1, &vertShader, NULL);
	glCompileShader(shader_name);

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << (shaderType == GL_VERTEX_SHADER ? "Error with vertex shader compilation: " : "Error with Fragment shader compilation:") << std::endl << infoLog << std::endl;
	}

	return shader_name;
};



std::string Shader::loadShaderSrc(const char* filename)
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

void Shader::activate()
{
	glUseProgram(id);
}

void Shader::setMat4(const std::string& name, glm::mat4 val)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Shader::setInt(const std::string& name, int val)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), val);
}

void Shader::setFloat(const std::string& name, float val)
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), val);
}

void Shader::set4Float(const std::string& name, float val1,float val2,float val3,float val4)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), val1,val2,val3,val4);
}

void Shader::setBool(const std::string& name, bool val)
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int) val);
}

void Shader::set3Float(const std::string& name, float v1, float v2, float v3) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}

void Shader::set3Float(const std::string& name, glm::vec3 v) {
	glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}
