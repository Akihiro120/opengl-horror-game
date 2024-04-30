#pragma once
#include "../../../../entities/entity.h"

class EquirectangularMap : public Entity
{
public:
	EquirectangularMap(Transform transform);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
};