#pragma once
#include "../item.h"

class ItemCube : public ItemActor
{
public:
	ItemCube(Transform transform, UpdateData& data);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;

	void onRayHoverInteract(std::vector<Entity*>& entities, int colliderID) override;
	void onStopRayHoverInteract(std::vector<Entity*>& entities) override;

	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;

private:
	bool m_Hovering = false;
};