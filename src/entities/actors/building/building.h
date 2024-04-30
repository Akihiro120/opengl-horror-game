#pragma once
#include "../../entity.h"

class Building : public Entity
{
public:
	Building(Transform transform);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;
};