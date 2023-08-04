#version 400 core

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

uniform float mixVal;

uniform Material material;
uniform PointLight pointLight;

uniform vec3 viewPos;


vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);

void main() {

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 diffMap = vec3(texture(material.diffuse, texCoord));
	vec3 specMap = vec3(texture(material.specular, texCoord));

	FragColor = vec4(calcPointLight(norm, viewDir, diffMap, specMap), 1.0);
}

vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{

	vec3 ambient = pointLight.ambient * diffMap;

	// diffuse
	vec3 lightDir = normalize(pointLight.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = pointLight.diffuse * (diff * diffMap);

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = pointLight.specular * (spec * specMap);

    return vec3(ambient + diffuse + specular);

}
