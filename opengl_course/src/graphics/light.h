#pragma once

#include <glm/glm.hpp>
#include "shader.h"

struct PointLight
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	void render(Shader shader);
};


struct DirLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	void render(Shader shader);
};
