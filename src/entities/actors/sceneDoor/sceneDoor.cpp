#include "sceneDoor.h"
#include "../../../physics/AABB/aabb.h"
#include "../camera/camera.h"
#include "../../../data/items/itemCollection.h"
#include "../../../data/math.h"
#include "../../../scene/scenes/sceneLoader.h"

SceneDoor::SceneDoor(Transform transform, UpdateData& data, int sceneIndex, std::string doorTag) : Entity(transform)
{
	this->sceneIndex = sceneIndex;
	this->m_DoorTag = doorTag;
	m_Colliders.push_back(AABBCollision::makeAABBColliderTransform(data.models->woodDoorModel.getMeshes()[1].getVertices(), transform));
}

void SceneDoor::update(UpdateData& data, std::vector<Entity*>& entities)
{

}

void SceneDoor::draw(UpdateData& data, std::vector<Entity*>& entities)
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

void SceneDoor::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
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

void SceneDoor::onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID)
{
	// check if the player owns the correct key
	Camera* cameraReference = dynamic_cast<Camera*>(entities[0]);
	if (cameraReference)
	{
		// check the inventory
		if (m_DoorTag != "None")
		{
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
							// move to next scene
							changeScene(data, entities, sceneIndex);
						}
					}
				}
			}
		}
		else {
			// move to next scene
			changeScene(data, entities, sceneIndex);
		}
	}
}

void SceneDoor::changeScene(UpdateData& data, std::vector<Entity*>& entities, int index)
{
	if (index == 1)
	{
		SceneLoader::SCENE_visual2(data, entities);
	}
}