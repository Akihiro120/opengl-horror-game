#include "equirectangular.h"

EquirectangularMap::EquirectangularMap(Transform transform) : Entity(transform)
{

}

void EquirectangularMap::draw(UpdateData& data, std::vector<Entity*>& entities)
{
	data.shaders->equirectangularShader.bindProgram();
	Textures::bindTexture2D(data.textures->hdriMap, GL_TEXTURE0);
	data.shaders->equirectangularShader.setInts("equirectangularMap", 0);

	data.models->cubeModel.draw();
}