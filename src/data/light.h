#pragma once
#include <glm/glm.hpp>

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 color;
};

struct PointLight
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(0.0f);

	unsigned int shadowFBO = 0;
	unsigned int shadowMap = 0;
	bool castShadows = false;
	float radius = 0.0f;

	bool destroy;
};