#include "texture.h"

#include <glbinding/gl/gl.h>

#include <iostream>

Texture::Texture(const GLchar * texturePath)
{
	glGenTextures(1, &ID);

	unsigned char* data = stbi_load(texturePath, &width, &height, &numChannels, 0);
	if (data)
	{
		GLenum format;
		if (numChannels == 1)
			format = GL_RED;
		if (numChannels == 3)
			format = GL_RGB;
		if (numChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD " << texturePath << std::endl;
	}
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}
