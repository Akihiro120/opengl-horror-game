#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "../../data/vertex.h"

class Mesh
{
public:
	Mesh();

	void setVertex(std::vector<Vertex> vertices);
	void setIndices(std::vector<unsigned int> indices);
	void buildMesh();
	void draw();

	std::vector<Vertex> getVertices();
	std::vector<unsigned int> getIndices();

private:
	unsigned int m_VAO, m_VBO, m_EBO;

	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
};