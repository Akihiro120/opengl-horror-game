#include "itemCube.h"
#include "../../../../data/items/itemCollection.h"
#include "../../../../physics/AABB/aabb.h"

ItemCube::ItemCube(Transform transform, UpdateData& data) : ItemActor(transform)
{
	m_Colliders.push_back(AABBCollision::makeAABBCollider(data.models->cubeModel.getMeshes()[0].getVertices(), m_Transform.location));
	currentItem = ItemCollection::ITEM_cube();
}

void ItemCube::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	if(m_Hovering)
	{ 
		data.shaders->lightShader.bindProgram();
		data.shaders->lightShader.setMat4("model", model);
		data.shaders->lightShader.setVec3("color", glm::vec3(1.0f));
	}
	else {
		data.shaders->defaultShader.bindProgram();
		data.shaders->defaultShader.setMat4("model", model);
		data.textures->loadBrickMaterial(&data.shaders->defaultShader, 1.0f);
		data.shaders->defaultShader.setFloats("tiling", m_Transform.scale.x);
	}

	data.models->cubeModel.draw();
}

void ItemCube::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);


	data.models->cubeModel.draw();
}

void ItemCube::onRayHoverInteract(std::vector<Entity*>& entities, int colliderID)
{
	m_Hovering = true;
}

void ItemCube::onStopRayHoverInteract(std::vector<Entity*>& entities)
{
	m_Hovering = false;
}
