#pragma once
#include "../../../entity.h"

class SafetyDoorSwitch : public Entity
{
public:
	SafetyDoorSwitch(Transform transform, UpdateData& data, int doorIndex);

	void update(UpdateData& data, std::vector<Entity*>& entities) override;
	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;

	void onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID) override;

private:
	int doorIndex = 0;
	bool m_Open = false;
	bool m_PerformAnimation = false;

	glm::vec3 m_HandleStart = glm::vec3(0.0f);
	glm::vec3 m_HandleEnd = glm::vec3(0.0f, -0.5f, 0.0f);
	float m_Percent = 0.0f;

	float deltaTime = 0.0f;
	float lastTime = 0.0f;
};