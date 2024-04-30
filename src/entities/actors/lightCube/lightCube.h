#pragma once
#include "../../entity.h"

class LightCube : public Entity
{
public:
	LightCube(Transform transform, glm::vec3 lightColor, UpdateData& data, bool castShadows);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;

private:
	glm::vec3 lightColor;
};