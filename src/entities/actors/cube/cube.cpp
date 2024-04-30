#include "cube.h"
#include "../../../physics/AABB/aabb.h"

Cube::Cube(Transform transform, UpdateData& data) : Entity(transform)
{
	//m_Colliders.push_back(AABBCollision::makeAABBCollider(data.models->cubeModel.getMeshes()[0].getVertices(), m_Transform.location));
}

void Cube::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);

	data.textures->loadBrickMaterial(&data.shaders->defaultShader, m_Transform.scale.x);

	data.models->cubeModel.draw();
}

void Cube::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*> entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);

	data.models->cubeModel.draw();
}