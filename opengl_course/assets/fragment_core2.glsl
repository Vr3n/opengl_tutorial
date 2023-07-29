#version 400 core
out vec4 FragColor;

// in vec3 ourColor;

void main ()
{
	float colorVerts[] = {
		1.0f, 1.25f, 0.7f,
		0.5f, 0.5f, 0.25f,
		0.4f, 0.2f, 1.0f,
	};

	FragColor = vec4(vec3(colorVerts[0], colorVerts[1], colorVerts[2]), 1.0);
}
