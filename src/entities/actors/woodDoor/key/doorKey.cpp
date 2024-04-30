#include "doorKey.h"
#include "../../../../physics/AABB/aabb.h"
#include "../../../../data/items/itemCollection.h"

DoorKey::DoorKey(Transform transform, UpdateData& data, std::string doorName) : ItemActor(transform)
{
	m_Colliders.push_back(AABBCollision::makeAABBColliderTransform(data.models->doorKeyModel.getMeshes()[0].getVertices(), transform));

	Item key = ItemCollection::ITEM_doorKey();
	key.tags.push_back(doorName);
	currentItem = key;
}

void DoorKey::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	if (m_Hovering)
	{
		data.shaders->lightShader.bindProgram();
		data.shaders->lightShader.setMat4("model", model);
		data.shaders->lightShader.setVec3("color", glm::vec3(1.0f));
	}
	else {
		data.shaders->defaultShader.bindProgram();
		data.shaders->defaultShader.setMat4("model", model);
		
		data.textures->loadIronMaterial(&data.shaders->defaultShader, 1.0f);
	}

	data.models->doorKeyModel.draw();
}

void DoorKey::onRayHoverInteract(std::vector<Entity*>& entities, int colliderID)
{
	m_Hovering = true;
}

void DoorKey::onStopRayHoverInteract(std::vector<Entity*>& entities)
{
	m_Hovering = false;
}