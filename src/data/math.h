#pragma once
#include <glm/glm.hpp>

namespace Math
{
	static float lerp(float x, float y, float t)
	{
		return x * (1.f - t) + y * t;
	}

	static glm::vec3 lerpVec3(glm::vec3 a, glm::vec3 b, float t)
	{
		return {
			a.x * (1.f - t) + b.x * t,
			a.y * (1.f - t) + b.y * t,
			a.z * (1.f - t) + b.z * t,
		};
	}
}