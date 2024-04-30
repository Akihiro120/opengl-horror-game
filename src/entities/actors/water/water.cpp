#include "water.h"

Water::Water(Transform transform) : Entity(transform)
{

}

void Water::draw(UpdateData& data, std::vector<Entity*>& entities) 
{
	glm::mat4 model = glm::mat4(1.0f);
	model = makeModelMatrix();

	data.shaders->defaultShader.bindProgram();
	data.shaders->defaultShader.setMat4("model", model);

	Textures::bindTexture2D(data.textures->reflectTexture, GL_TEXTURE0);
	Textures::bindTexture2D(data.textures->water_NORMAL, GL_TEXTURE1);
	Textures::bindTexture2D(data.textures->macro_VARIATION, GL_TEXTURE2);
	Textures::bindTexture2D(data.textures->DUDV, GL_TEXTURE3);

	data.shaders->defaultShader.setInts("useNormalTexture", 1);
	data.shaders->defaultShader.setInts("normalTexture", 1);
	data.shaders->defaultShader.setInts("dudvTexture", 3);
	data.shaders->defaultShader.setFloats("time", glfwGetTime());

	if (data.shaders->enableReflections)
	{
		data.shaders->defaultShader.setInts("useAlbedoTexture", 1);
		data.shaders->defaultShader.setInts("albedoTexture", 0);
	} else{
		data.shaders->defaultShader.setInts("useAlbedoTexture", 0);
		data.shaders->defaultShader.setVec3("baseColor", glm::vec3(0.0f, 0.0f, 1.0f));
	}

	data.shaders->defaultShader.setInts("microTexture", 2);
	data.shaders->defaultShader.setFloats("tiling", 10.0f);
	data.shaders->defaultShader.setFloats("heightScale", 0.1f);

	data.shaders->defaultShader.setFloats("metallic", 0.7f);
	data.shaders->defaultShader.setFloats("roughness", 0.3f);

	data.shaders->defaultShader.setInts("renderAsWater", true);
	data.models->waterPlaneModel.draw();
	data.shaders->defaultShader.setInts("renderAsWater", false);
}