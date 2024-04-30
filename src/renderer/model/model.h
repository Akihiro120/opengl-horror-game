#pragma once
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../mesh/mesh.h"

class Model
{
public:
	Model(std::string filePath);

	void buildModel(std::string filePath);
	void draw();

	std::vector<Mesh> getMeshes();
	
private:
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Mesh> meshes;
};