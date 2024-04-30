#pragma once
#include "../../../entity.h"

class Building2 : public Entity
{
public:
	Building2(Transform transform);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;
};