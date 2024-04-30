#pragma once
#include "../../entity.h"

class Water : public Entity
{
public:
	Water(Transform transform);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
};