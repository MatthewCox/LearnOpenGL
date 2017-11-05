#ifndef __MESH_H__
#define __MESH_H__

#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct MeshTexture
{
	MeshTexture(const GLchar * texturePath)
		: texture(texturePath)
	{
	}

	Texture texture;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture*> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture*> textures);
	void draw(Shader shader);

private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif // !__MESH_H__
