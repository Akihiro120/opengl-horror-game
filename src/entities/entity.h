#pragma once
#include <vector>
#include "../data/transform.h"
#include "../data/updateData.h"
#include "../data/aabb.h"

class Entity
{
public:
	Entity(Transform transform);

	virtual void draw(UpdateData& data, std::vector<Entity*>& entities);
	virtual void update(UpdateData& data, std::vector<Entity*>& entities);
	virtual void userInterface(UpdateData& data, std::vector<Entity*>& entities);

	// interaction
	virtual void onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID);

	virtual void onRayHoverInteract(std::vector<Entity*>& entities, int colliderID);
	virtual void onStopRayHoverInteract(std::vector<Entity*>& entities);

	virtual void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities);

	Transform getTransform();
	std::vector<AABB> getColliders();

	void destroyActor();

	bool getDestroyState()
	{
		return m_Destroy;
	}

private:

protected:
	glm::mat4 makeModelMatrix();

	Transform m_Transform;
	std::vector<AABB> m_Colliders;
	bool m_Destroy = false;
};