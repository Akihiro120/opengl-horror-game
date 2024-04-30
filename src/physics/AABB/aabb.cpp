#include "aabb.h"
#include <glm/gtc/matrix_transform.hpp>

AABB AABBCollision::makeAABBCollider(std::vector<Vertex> vertices, glm::vec3 location)
{
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());

	for (auto& vertex : vertices)
	{
		min = glm::min(min, vertex.vertices);
		max = glm::max(max, vertex.vertices);
	}

	// offset
	min += location;
	max += location;

	return { min, max };
}

AABB AABBCollision::makeAABBColliderTransform(std::vector<Vertex> vertices, Transform transform)
{
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());

	glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	for (auto& vertex : vertices)
	{
		min = glm::min(min, glm::vec3(rotate * glm::vec4(vertex.vertices, 1.0f)));
		max = glm::max(max, glm::vec3(rotate * glm::vec4(vertex.vertices, 1.0f)));
	}

	// offset
	min += transform.location;
	max += transform.location;

	return { min, max };
}

bool AABBCollision::checkAABBCollision(AABB& aabb1, AABB& aabb2)
{
	return aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x &&
		aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y &&
		aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z;
}