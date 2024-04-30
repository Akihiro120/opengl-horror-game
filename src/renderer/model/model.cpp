#include "model.h"
#include <iostream>

Model::Model(std::string filePath)
{
	buildModel(filePath);
}

void Model::buildModel(std::string filePath)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Assimp Error: " << import.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}

void Model::draw()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].draw();
	}
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

static glm::vec3 assimpToGlmVec3(aiVector3D vector)
{
	return glm::vec3(vector.x, vector.y, vector.z);
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// vertices
		vertex.vertices = assimpToGlmVec3(mesh->mVertices[i]);
		vertex.normals = assimpToGlmVec3(mesh->mNormals[i]);
		vertex.tangents = assimpToGlmVec3(mesh->mTangents[i]);

		// texture coords
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Mesh model{};
	model.setVertex(vertices);
	model.setIndices(indices);
	model.buildMesh();

	return model;
}

std::vector<Mesh> Model::getMeshes()
{
	return meshes;
}