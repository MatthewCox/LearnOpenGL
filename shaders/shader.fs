#version 330 core

out vec4 FragColour;

in vec3 vertexColour;
in vec2 texCoord;

uniform sampler2D diffuseTexture;

void main()
{
	FragColour = texture(diffuseTexture, texCoord) * vec4(vertexColour, 1.0);
}