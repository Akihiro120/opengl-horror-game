#pragma once
#include "../../entity.h"
#include "../../../data/item.h"

class ItemActor : public Entity
{
public:
	ItemActor(Transform transform);

	void onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID) override;

protected:
	Item currentItem;
};