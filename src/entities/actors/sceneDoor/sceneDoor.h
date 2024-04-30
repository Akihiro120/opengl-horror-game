#pragma once
#include "../../entity.h"

class SceneDoor : public Entity
{
public:
	SceneDoor(Transform transform, UpdateData& data, int sceneIndex, std::string doorTag);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void update(UpdateData& data, std::vector<Entity*>& entities) override;

	void onRayInteract(UpdateData& data, std::vector<Entity*>& entities, int colliderID) override;

	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;
	void changeScene(UpdateData& data, std::vector<Entity*>& entities, int index);

private:
	bool m_Open = false;
	bool m_PerformAnimation = false;

	float m_StartingRotation = 0.0f;
	float m_EndRotation = 90.0f;
	float m_Percent = 0.0f;

	float m_DeltaTime = 0.0f;
	float m_LastTime = 0.0f;

	int sceneIndex;
	std::string m_DoorTag;
};