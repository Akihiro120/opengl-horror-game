#include "doorSwitch.h"
#include "../../../../physics/AABB/aabb.h"
#include "../../../../data/math.h"
#include "../door/door.h"

SafetyDoorSwitch::SafetyDoorSwitch(Transform transform, UpdateData& data, int doorIndex) : Entity(transform)
{
	m_Colliders.push_back(AABBCollision::makeAABBColliderTransform( data.models->safetyDoorHandleModel.getMeshes()[0].getVertices(), transform));

	this->doorIndex = doorIndex;
}

void SafetyDoorSwitch::update(UpdateData& data, std::vector<Entity*>& entities)
{
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	// perform animation
	if (m_PerformAnimation)
	{
		if (m_Open)
		{
			// close door
			m_Percent = glm::min(glm::max(0.0f, m_Percent + (5.0f * deltaTime)), 1.0f);
			if (m_Percent == 0)
			{
				m_PerformAnimation = false;
			}
		}
		else {
			// open door
			m_Percent = glm::min(glm::max(0.0f, m_Percent - (5.0f * deltaTime)), 1.0f);
			if (m_Percent == 1)
			{
				m_PerformAnimation = false;
			}
		}
	}
}

void SafetyDoorSwitch::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);

	data.textures->loadIronMaterial(&data.shaders->defaultShader, 1.0f);

	data.models->safetyDoorHandleModel.getMeshes()[0].draw();

	// handle
	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);
	data.shaders->defaultShader.setInts("useAlbedoTexture", 0);
	data.shaders->defaultShader.setInts("useNormalTexture", 0);
	data.shaders->defaultShader.setFloats("roughness", 0.5f);
	data.shaders->defaultShader.setFloats("metallic", 0.5f);
	data.shaders->defaultShader.setInts("useEmissive", true);
	if (m_Open)
	{
		data.shaders->defaultShader.setVec3("baseColor", glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		data.shaders->defaultShader.setVec3("baseColor", glm::vec3(1.0f, 0.0f, 0.0f));
	}
	data.shaders->defaultShader.setFloats("emissiveStrength", 0.5f);
	data.shaders->defaultShader.setMat4("model", glm::translate(model, Math::lerpVec3(m_HandleStart, m_HandleEnd, m_Percent)));
	data.models->safetyDoorHandleModel.getMeshes()[2].draw();

	data.shaders->defaultShader.setInts("useEmissive", false);
	data.textures->loadSteelMaterial(&data.shaders->defaultShader, 1.0f);

	data.models->safetyDoorHandleModel.getMeshes()[1].draw();
}

void SafetyDoorSwitch::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);

	data.models->safetyDoorHandleModel.getMeshes()[0].draw();

	shader->setMat4("model", glm::translate(model, Math::lerpVec3(m_HandleStart, m_HandleEnd, m_Percent)));
	data.models->safetyDoorHandleModel.getMeshes()[1].draw();
	data.models->safetyDoorHandleModel.getMeshes()[2].draw();
}

void SafetyDoorSwitch::onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID)
{
	if (m_Open)
	{
		m_Open = false;
		m_PerformAnimation = true;
	}
	else {
		m_Open = true;
		m_PerformAnimation = true;
	}

	for (int i = 1; i < entities.size(); i++)
	{
		// check if it is a door
		SafetyDoor* doorReference = dynamic_cast<SafetyDoor*>(entities[i]);
		if (doorReference)
		{
			if (doorReference->getIndex() == doorIndex)
			{
				doorReference->setOpen(m_Open);
				doorReference->setPlayAnimation(true);
			}
		}
	}
}