#version 330 core

out vec4 FragColour;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 viewPos;
uniform vec3 lightColour;
uniform vec3 lightPos;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
};
uniform Material material;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColour;

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColour * vec3(texture(material.diffuse, texCoord));

	// specular
	float specularStrength = 1.0;
	float shininess = 32.0;
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColour * vec3(texture(material.specular, texCoord));

	FragColour = vec4(ambient + diffuse + specular, 1.0);
}
