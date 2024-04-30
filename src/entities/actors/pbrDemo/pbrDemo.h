#pragma once
#include "../../entity.h"

class PBRDemo : public Entity
{
public:
	PBRDemo(Transform transform, UpdateData& data, std::vector<Entity*>& entities);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;
};