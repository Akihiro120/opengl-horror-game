#include "woodDoor.h"
#include "../../../physics/AABB/aabb.h"
#include "../camera/camera.h"
#include "../../../data/items/itemCollection.h"
#include "../../../data/math.h"

WoodDoor::WoodDoor(Transform transform, UpdateData& data, std::string doorTag) : Entity(transform)
{
	this->m_DoorTag = doorTag;
	m_Colliders.push_back(AABBCollision::makeAABBColliderTransform(data.models->woodDoorModel.getMeshes()[1].getVertices(), transform));
}

void WoodDoor::update(UpdateData& data, std::vector<Entity*>& entities)
{
	// delta time
	float currentTime = glfwGetTime();
	m_DeltaTime = currentTime - m_LastTime;
	m_LastTime = currentTime;

	// animation
	// linear - max(0, percent +- 0.05);
	// bezier - max(0, percent - max((percent / 10.0f), 0.005f)); // in
	// bezier - min(1, percent + max(((1.0f - percent) / 10.0f), 0.005f)); // out

	if (m_PerformAnimation)
	{
		if (m_Open)
		{
			// open door
			m_Percent = glm::clamp(glm::min(1.0f, m_Percent + glm::max(((1.0f - m_Percent) / 10.0f), 0.005f) * (m_DeltaTime * 20.0f)), 0.0f, 1.0f);
		}
	}

	// collision
	if (m_Percent == 1.0f)
	{
		m_Colliders[0].activeCollider = false;
		m_PerformAnimation = false;
	}
}

void WoodDoor::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);

	data.textures->loadPlankMaterial(&data.shaders->defaultShader, 1.0f);

	// frame
	data.models->woodDoorModel.getMeshes()[1].draw();

	// door
	data.shaders->defaultShader.setMat4("model", glm::rotate(model, glm::radians(Math::lerp(m_StartingRotation, m_EndRotation, m_Percent)), glm::vec3(0.0f, 1.0f, 0.0f)));
	data.models->woodDoorModel.getMeshes()[0].draw();
}

void WoodDoor::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);

	// frame
	data.models->woodDoorModel.getMeshes()[1].draw();

	// door
	shader->setMat4("model", glm::rotate(model, glm::radians(Math::lerp(m_StartingRotation, m_EndRotation, m_Percent)), glm::vec3(0.0f, 1.0f, 0.0f)));
	data.models->woodDoorModel.getMeshes()[0].draw();
}

void WoodDoor::onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID)
{
	// check if the player owns the correct key
	Camera* cameraReference = dynamic_cast<Camera*>(entities[0]);
	if (cameraReference)
	{
		// check the inventory
		for (int i = 0; i < cameraReference->getInventory().size(); i++)
		{
			// check for the key
			if (cameraReference->getInventory()[i].name == ItemCollection::ITEM_doorKey().name)
			{
				// check the tag for equal tag
				for (int j = 0; j < cameraReference->getInventory()[i].tags.size(); j++)
				{
					if (cameraReference->getInventory()[i].tags[j] == m_DoorTag)
					{
						//std::cout << "You have the correct key" << std::endl;
						m_PerformAnimation = true;
						m_Open = true;
						break;
					}
				}
			}
		}
	}
}