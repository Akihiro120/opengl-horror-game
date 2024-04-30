#include "quad.h"

Quad::Quad(Transform transform) : Entity(transform)
{
	std::vector<unsigned int> indices = {
	0, 1, 3, 3, 1, 2
	};

	std::vector<Vertex> vertices = {
		Vertex{glm::vec3(-0.5f, 0.5f, 0.0f)},
		Vertex{glm::vec3(-0.5f, -0.5f, 0.0f)},
		Vertex{glm::vec3(0.5f, -0.5f, 0.0f)},
		Vertex{glm::vec3(0.5f, 0.5f, 0.0f)},
	};

	mesh.setVertex(vertices);
	mesh.setIndices(indices);
	mesh.buildMesh();
}

void Quad::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", glm::mat4(1.0f));
	mesh.draw();
}