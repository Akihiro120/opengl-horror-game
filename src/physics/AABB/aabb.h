#pragma once
#include <vector>
#include <limits>
#include "../../data/aabb.h"
#include "../../data/vertex.h"
#include "../../data/transform.h"

class AABBCollision
{
public:
	static AABB makeAABBCollider(std::vector<Vertex> vertices, glm::vec3 location);
	static AABB makeAABBColliderTransform(std::vector<Vertex> vertices, Transform transform);
	static bool checkAABBCollision(AABB& aabb1, AABB& aabb2);
};