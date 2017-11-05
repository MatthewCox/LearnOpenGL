#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "stb_image.h"

#include <glad/glad.h>

#include <string>

class Texture
{
private:
	unsigned int ID;
	int width = 0;
	int height = 0;
	int numChannels = 0;

public:
	Texture(const GLchar* texturePath);
	void use();
};

#endif // !__TEXTURE_H__