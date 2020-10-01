#pragma once

#include "glm.hpp"

class AABB
{
public:
	float x, y, z, xs, ys, zs;

	AABB() = default;
	AABB(float x, float y, float z, float xs, float ys, float zs);
};

bool collide(AABB box, glm::vec3 point);