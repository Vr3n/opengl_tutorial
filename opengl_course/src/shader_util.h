#pragma once
#include <glad/glad.h>

std::string loadShaderSrc(const char* filename);
void compileShader(unsigned int &shader_name, const char* shader_file_path, GLenum shaderType);
