#include "building.h"

Building::Building(Transform transform) : Entity(transform)
{

}

void Building::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);

	data.textures->loadConcreteMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel.getMeshes()[1].draw();

	data.textures->loadBrickMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel.getMeshes()[3].draw();

	data.textures->loadIronMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel.getMeshes()[0].draw();

	data.textures->loadRustMaterial(&data.shaders->defaultShader, 1.0f);
	data.models->buildingModel.getMeshes()[2].draw();

	//std::cout << data.models->buildingModel.getMeshes().size();
}

void Building::drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	shader->bindProgram();
	shader->setMat4("model", model);

	/*data.models->buildingModel.getMeshes()[1].draw();

	data.models->buildingModel.getMeshes()[2].draw();*/   
	data.models->buildingModel.draw();
}