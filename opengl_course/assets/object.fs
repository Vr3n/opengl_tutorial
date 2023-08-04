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

	float k0;
	float k1;
	float k2;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;

	float k0;
	float k1;
	float k2;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 texCoord;

uniform float mixVal;

uniform Material material;
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;


vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);
vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap);

void main() {

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 diffMap = vec3(texture(material.diffuse, texCoord));
	vec3 specMap = vec3(texture(material.specular, texCoord));

	FragColor = vec4(calcSpotLight(norm, viewDir, diffMap, specMap), 1.0);
}

vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{

	vec3 ambient = dirLight.ambient * diffMap;

	// diffuse
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = dirLight.diffuse * (diff * diffMap);

	// specular.
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = dirLight.specular * (spec * specMap);

    return vec3(ambient + diffuse + specular);

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


	float dist = length(pointLight.position - FragPos);
	float attenuation = 1.0 / (pointLight.k0 + pointLight.k1 * dist + pointLight.k2 * (dist * dist));

    return vec3(ambient + diffuse + specular) * attenuation;

}

vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffMap, vec3 specMap)
{
	vec3 lightDir = normalize(spotLight.position - FragPos);

	float theta = dot(lightDir, normalize(-spotLight.direction));


	// ambient.
	vec3 ambient = spotLight.ambient * diffMap;


	if (theta > spotLight.outerCutOff) // because using cosines and not degrees.
	{
		// if cutoff, render Light.


		// diffuse.
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = spotLight.diffuse * (diff * diffMap);

		// specular.
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);

		vec3 specular = spotLight.specular * (spec * specMap);


		float intensity = clamp((theta - spotLight.outerCutOff) / (spotLight.cutOff - spotLight.outerCutOff), 0.0, 1.0);

		diffuse *= intensity;
		specular *= intensity;

		float dist = length(spotLight.position - FragPos);
		float attenuation = 1.0 / (spotLight.k0 + spotLight.k1 * dist + spotLight.k2 * (dist * dist));


		return vec3(ambient + diffuse + specular) * attenuation;

	}
	else
	{
		return ambient;
	}

}
