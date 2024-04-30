#pragma once
#include "../../../entity.h"

class SafetyDoor : public Entity
{
public:
	SafetyDoor(Transform transform, UpdateData& data, int index);

	void draw(UpdateData& data, std::vector<Entity*>& entities) override;
	void update(UpdateData& data, std::vector<Entity*>& entities) override;
	void drawToShaderMap(UpdateData& data, Shader* shader, std::vector<Entity*>& entities) override;

	float getPercent()
	{
		return percent;
	}

	bool getOpen()
	{
		return m_Open;
	}

	void setPlayAnimation(bool play)
	{
		this->performAnimation = play;
	}

	void setOpen(bool open)
	{
		this->m_Open = open;
	}

	int getIndex()
	{
		return doorIndex;
	}

private:
	bool m_Open = false;
	bool DELEGATE_open = false;
	int m_LightIndex = 0;
	glm::vec3 m_LightPoint = glm::vec3(2.1f, 5.0f, 0.0f);
	glm::vec3 m_LightPosition = glm::vec3(0.0f);

	// animation
	glm::vec3 doorLeftStart = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 doorLeftEnd = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 doorLeftOffset = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 doorRightStart = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 doorRightEnd = glm::vec3(0.0f, 0.0f, -2.0f);
	glm::vec3 doorRightOffset = glm::vec3(0.0f, 0.0f, 0.0f);
	float percent = 0.0f;
	bool performAnimation = false;

	float deltaTime = 0.0f;
	float lastTime = 0.0f;

	int doorIndex = 0;
};