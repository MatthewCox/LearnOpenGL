#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader
{
private:
	unsigned int ID; // GL shader program ID

public:
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath);

	void use();

	void setBool(const std::string & name, bool value) const;
	void setInt(const std::string & name, int value) const;
	void setFloat(const std::string & name, float value) const;

	void setVec2(const std::string & name, float x, float y) const;
	void setVec2(const std::string & name, const glm::vec2 & v) const;
	void setVec3(const std::string & name, float x, float y, float z) const;
	void setVec3(const std::string & name, const glm::vec3 & v) const;
	void setVec4(const std::string & name, float x, float y, float z, float w) const;
	void setVec4(const std::string & name, const glm::vec4 & v) const;

	void setMat2(const std::string & name, const glm::mat2 & m) const;
	void setMat3(const std::string & name, const glm::mat3 & m) const;
	void setMat4(const std::string & name, const glm::mat4 & m) const;

private:
	static void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // !__SHADER_H__
