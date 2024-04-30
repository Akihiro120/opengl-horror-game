#include "raycasting.h"

Ray RayCasting::createRay(glm::vec3 start, glm::vec3 end)
{
	glm::vec3 origin = start;
	glm::vec3 direction = end;

	return { origin, direction };
}

bool RayCasting::checkAABBRayIntersection(Ray* ray, AABB* aabb, float range)
{
	glm::vec3 tMin = (aabb->min - ray->origin) / ray->direction;
	glm::vec3 tMax = (aabb->max - ray->origin) / ray->direction;

	glm::vec3 t1 = glm::min(tMin, tMax);
	glm::vec3 t2 = glm::max(tMin, tMax);

	float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
	float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

	// interaction test
	if (tNear > tFar || tFar < 0.0f)
	{
		return false;
	}

	// ray check
	if (tNear <= range)
	{
		return true;
	}
	else {
		return false;
	}
}