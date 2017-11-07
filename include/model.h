#ifndef __MODEL_H__
#define __MODEL_H__

#include <map>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"
#include "texture.h"

class Model
{
public:
	Model(const std::string & path);
	void draw(Shader & shader) const;

private:
	std::vector<Mesh> meshes;
	std::map<std::string, MeshTexture> texturesLoaded;
	std::string directory;
	void loadModel(const std::string & path);
	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh * mesh, const aiScene * scene);
	std::vector<MeshTexture*> loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName);

};

#endif // !__MODEL_H__
