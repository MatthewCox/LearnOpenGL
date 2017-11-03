#include "stb_image.h"

#include <glad/glad.h>

#include <string>

class Texture
{
public:
	unsigned int ID;
	int width = 0;
	int height = 0;
	int numChannels = 0;

	Texture(const GLchar* texturePath);
	void use();
};