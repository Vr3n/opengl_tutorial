#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

class Texture
{
public:
	Texture();
	Texture(const char* path, const char* name, bool defaultParams = true);

	void generate();
	void load(bool flip = true);

	void setFilters(GLenum all);
	void setFilters(GLenum mag, GLenum min);

	void setWrap(GLenum all);
	void setWrap(GLenum s, GLenum t);

	void activate();
	void bind();

	// texture object.
	int id;
	unsigned int texture;
	const char* name;

private:
	static int currentId;

	const char* path;
	int width;
	int height;
	int nChannels;
};
