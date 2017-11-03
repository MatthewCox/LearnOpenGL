#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>

#include <string>

class Shader
{
public:
	unsigned int ID; // GL shader program ID

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

	void setFloat3(const std::string &name, float v0, float v1, float v2) const;
	void setFloat4(const std::string &name, float v0, float v1, float v2, float v3) const;

private:
	static void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // !__SHADER_H__
