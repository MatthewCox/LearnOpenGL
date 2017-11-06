#version 330 core

out vec4 FragColour;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 viewPos;
uniform vec3 lightColour;
uniform vec3 lightPos;

uniform sampler2D texture_diffuse;
//uniform sampler2D texture_specular;

void main()
{
	// ambient
	float ambientStrength = 0.1;
	vec4 ambient = vec4(ambientStrength * lightColour, 1.0);

	// diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = vec4(diff * lightColour, 1.0);

	// specular
	float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = vec4(specularStrength * spec * lightColour, 1.0);

	FragColour = (ambient + diffuse + specular) * texture(texture_diffuse, texCoord);
}
