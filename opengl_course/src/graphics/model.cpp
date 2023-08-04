#include "model.h"

Model::Model () {}

void Model::init() { }

void Model::render(Shader shader)
{
	mesh.render(shader);
}


void Model::cleanup()
{
	mesh.cleanup();
}
