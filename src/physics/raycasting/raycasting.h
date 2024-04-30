#pragma once
#include <glm/glm.hpp>
#include "../../data/ray.h"
#include "../../data/aabb.h"

class RayCasting
{
public:
	static Ray createRay(glm::vec3 start, glm::vec3 end);
	static bool checkAABBRayIntersection(Ray* ray, AABB* aabb, float range);
};