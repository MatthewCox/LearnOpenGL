#version 330 core

out vec4 FragColour;

in vec2 texCoord;

uniform sampler2D texture_diffuse;
//uniform sampler2D texture_specular;

void main()
{
	FragColour = texture(texture_diffuse, texCoord);
}
