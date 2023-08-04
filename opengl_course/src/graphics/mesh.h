#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

struct Vertex 
{
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;

	static std::vector<struct Vertex> genList(float* vertices, int noVertices);
};

typedef struct Vertex Vertex;


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	std::vector<Texture> textures;

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void render(Shader shader);
	void cleanup();

private:
	unsigned int VAO, VBO, EBO;

	void setup();
};
