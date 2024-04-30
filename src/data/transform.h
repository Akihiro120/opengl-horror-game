#pragma once
#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale = glm::vec3(1.0f);

	Transform() = default;
	Transform(glm::vec3 location)
	{
		this->location = location;
	}

	Transform(glm::vec3 location, glm::vec3 rotation)
	{
		this->location = location;
		this->rotation = rotation;
	}

	Transform(glm::vec3 location, glm::vec3 rotation, glm::vec3 scale)
	{
		this->location = location;
		this->rotation = rotation;
		this->scale = scale;
	}
};