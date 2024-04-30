#pragma once
#include "../../entity.h"
#include "../../../data/item.h"

class Camera : public Entity
{
public:
	Camera(Transform transform, UpdateData& data);

	void update(UpdateData& data, std::vector<Entity*>& entities);
	void userInterface(UpdateData& data, std::vector<Entity*>& entities);

	void addItemToInventory(Item item);
	void removeItemAtIndex(int index);

	std::vector<Item> getInventory()
	{
		return m_Inventory;
	}
	glm::vec3 calculateDirection(float yaw, float pitch);
	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;

private:
	void movement(UpdateData& data, std::vector<Entity*>& entities);
	void mouse(UpdateData& data);
	void interaction(UpdateData& data, std::vector<Entity*>& entities);
	bool DELEGATE_rayIntersection = false;
	bool DELEGATE_interact = false;
	int m_CurrentInteractingActor = 0;
	int m_CurrentInteractingCollider = 0;

	float m_CameraSpeed = 5.0f;
	float m_Sensitivity = 0.1f;

	float m_DeltaTime = 0.0f;
	float m_LastTime = 0.0f;

	float m_LastX = 400;
	float m_LastY = 300;

	float floorLevel = 0.0f;

	bool DELEGATE_itemTesting = false;
	bool m_InventoryOpen = false;
	std::vector<Item> m_Inventory;
	bool m_Mouse = false;
};