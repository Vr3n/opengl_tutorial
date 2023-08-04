#pragma once

#include "cube.hpp"
#include "../light.h"

class Lamp : public Cube
{
public:
	glm::vec3 lightColor;

	// light strength values.
	PointLight pointLight;


	Lamp(
		glm::vec3 lightColor,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		glm::vec3 pos,
		glm::vec3 size,
		float k0,
		float k1,
		float k2
	) : lightColor(lightColor),
		pointLight({ pos, ambient, diffuse, specular, k0, k1, k2 }),
		Cube(Material::white_plastic, pos, size) 
	{ }

	void render(Shader shader)
	{
		// set light color.
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}

};