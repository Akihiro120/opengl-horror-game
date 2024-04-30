#include "door.h"
#include "../../../../physics/AABB/aabb.h"
#include "../../../../data/math.h"

SafetyDoor::SafetyDoor(Transform transform, UpdateData& data, int index) : Entity(transform)
{
	m_Colliders.push_back(AABBCollision::makeAABBColliderTransform(data.models->safetyDoorColliderModel.getMeshes()[0].getVertices(), m_Transform));

	doorIndex = index;

	// add light
	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 origin = glm::translate(glm::mat4(1.0f), transform.location);
	m_LightPosition = glm::vec3(origin * rotate * glm::vec4(m_LightPoint, 1.0f));
	m_LightIndex = data.shaders->addPointLight(PointLight{glm::vec3(m_LightPosition), glm::vec3(2.0f, 0.0f, 0.0f), 0, 0, true});
}

void SafetyDoor::update(UpdateData& data, std::vector<Entity*>& entities)
{
	if(m_LightIndex < data.shaders->pointLights.size() - 1)
	if (m_Open)
	{
		data.shaders->setPointLight(PointLight{ glm::vec3(m_LightPosition), glm::vec3(0.0f, 0.5f, 0.0f), data.shaders->pointLights[m_LightIndex].shadowFBO, data.shaders->pointLights[m_LightIndex].shadowMap, true}, m_LightIndex);
	}
	else {
		data.shaders->setPointLight(PointLight{ glm::vec3(m_LightPosition), glm::vec3(0.5f, 0.0f, 0.0f), data.shaders->pointLights[m_LightIndex].shadowFBO, data.shaders->pointLights[m_LightIndex].shadowMap, true }, m_LightIndex);
	}

	// animation
	// linear - max(0, percent +- 0.05);
	// bezier - max(0, percent - max((percent / 10.0f), 0.005f)); // in
	// bezier - min(1, percent + max(((1.0f - percent) / 10.0f), 0.005f)); // out
	float currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;
	if (performAnimation)
	{
		if (m_Open)
		{
			// open door
			percent = glm::clamp(glm::max(0.0f, percent + (5.0f * deltaTime)), 0.0f, 1.0f);
		}	
		else {
			// close door
			percent = glm::clamp(glm::max(0.0f, percent - (5.0f * deltaTime)), 0.0f, 1.0f);
		}
	}

	if (percent == 1.0f)
	{
		m_Colliders[0].activeCollider = false;
		performAnimation = false;
	}
	if (percent == 0.0f)
	{
		m_Colliders[0].activeCollider = true;
		performAnimation = false;
	}
}

void SafetyDoor::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);
	
	data.textures->loadIronMaterial(&data.shaders->defaultShader, 1.0f);

	// frame
	data.models->safetyDoorModel.getMeshes()[1].draw();

	// panels
	data.shaders->defaultShader.setMat4("model", glm::translate(model, Math::lerpVec3(doorLeftStart, doorLeftEnd, percent)));
	data.models->safetyDoorModel.getMeshes()[0].draw();

	data.shaders->defaultShader.setMat4("model", glm::translate(model, Math::lerpVec3(doorRightStart, doorRightEnd, percent)));
	data.models->safetyDoorModel.getMeshes()[2].draw();

	// light
	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);
	data.shaders->defaultShader.setInts("useAlbedoTexture", 0);
	data.shaders->defaultShader.setInts("useNormalTexture", 0);
	data.shaders->defaultShader.setFloats("roughness", 0.5f);
	data.shaders->defaultShader.setFloats("metallic", 0.5f);
	if (m_Open)
	{
		data.shaders->defaultShader.setVec3("baseColor", glm::vec3(0.0f, 1.0f, 0.0f));
		data.shaders->defaultShader.setInts("useEmissive", true);
	}
	else {
		data.shaders->defaultShader.setVec3("baseColor", glm::vec3(1.0f, 0.0f, 0.0f));
		data.shaders->defaultShader.setInts("useEmissive", true);
	}
	data.shaders->defaultShader.setFloats("emissiveStrength", 0.5f);
	data.models->safetyDoorModel.getMeshes()[3].draw();

	data.shaders->defaultShader.setInts("useEmissive", false);
}

void SafetyDoor::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);

	// frame
	data.models->safetyDoorModel.getMeshes()[1].draw();

	// panels
	shader->setMat4("model", glm::translate(model, Math::lerpVec3(doorLeftStart, doorLeftEnd, percent)));
	data.models->safetyDoorModel.getMeshes()[0].draw();

	shader->setMat4("model", glm::translate(model, Math::lerpVec3(doorRightStart, doorRightEnd, percent)));
	data.models->safetyDoorModel.getMeshes()[2].draw();
}
