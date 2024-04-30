#include "lightCube.h"

LightCube::LightCube(Transform transform, glm::vec3 lightColor, UpdateData& data, bool castShadows) : Entity(transform)
{
	this->lightColor = lightColor;

	data.shaders->addPointLight(PointLight{ m_Transform.location, lightColor * 2.5f, 0, 0, castShadows });
}

void LightCube::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);

	data.textures->loadIronMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->cageLightModel.getMeshes()[0].draw();
	data.models->cageLightModel.getMeshes()[2].draw();


	data.shaders->defaultShader.setInts("useAlbedoTexture", 0);
	data.shaders->defaultShader.setInts("useNormalTexture", 0);
	data.shaders->defaultShader.setFloats("roughness", 0.5f);
	data.shaders->defaultShader.setFloats("metallic", 0.5f);

	data.shaders->defaultShader.setVec3("baseColor", lightColor);
	data.shaders->defaultShader.setInts("useEmissive", true);

	data.shaders->defaultShader.setFloats("emissiveStrength", 1.0f);
	data.models->cageLightModel.getMeshes()[1].draw();

	data.shaders->defaultShader.setInts("useEmissive", false);
}

void LightCube::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);

	data.models->cageLightModel.getMeshes()[0].draw();
	data.models->cageLightModel.getMeshes()[2].draw();
}