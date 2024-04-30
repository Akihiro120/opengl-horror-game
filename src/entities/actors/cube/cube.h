#pragma once
#include "../../entity.h"

class Cube : public Entity
{
public:
	Cube(Transform transform, UpdateData& data);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*> entities);

private:

};