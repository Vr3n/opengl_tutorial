#pragma once

#include <glm/glm.hpp>
#include "shader.h"

struct PointLight
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// attenuation consts.
	float k0;
	float k1;
	float k2;


	void render(Shader shader, int idx);
};


struct DirLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;


	void render(Shader shader);
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;

	float cutOff;
	float outerCutOff;


	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// attenuation consts.
	float k0;
	float k1;
	float k2;

	void render(Shader shader, int idx);


}; 