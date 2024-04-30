#include "building2.h"

Building2::Building2(Transform transform) : Entity(transform)
{

}

void Building2::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);

	data.textures->loadRustMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel2.getMeshes()[1].draw();

	data.textures->loadBrickMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel2.getMeshes()[2].draw();

	data.textures->loadConcreteMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel2.getMeshes()[0].draw();

	data.textures->loadBrickMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel2.getMeshes()[3].draw();

	//std::cout << data.models->buildingModel.getMeshes().size();
}

void Building2::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);

	data.models->buildingModel2.draw();
}