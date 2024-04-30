#pragma once
#include "../../item/item.h"

class DoorKey : public ItemActor
{
public:
	DoorKey(Transform transform, UpdateData& data, std::string doorName);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	virtual void onRayHoverInteract(std::vector<Entity*>& entities, int colliderID);
	virtual void onStopRayHoverInteract(std::vector<Entity*>& entities);

private:
	bool m_Hovering = false;
};