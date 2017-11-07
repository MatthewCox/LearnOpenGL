#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <glbinding/gl/gl.h>

#include <string>

#include "stb_image.h"

using namespace gl;

class Texture
{
private:
	unsigned int ID;
	int width = 0;
	int height = 0;
	int numChannels = 0;

public:
	Texture(const GLchar * texturePath);
	void use();
};

#endif // !__TEXTURE_H__
