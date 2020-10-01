#include "collide.h"

AABB::AABB(float x, float y, float z, float xs, float ys, float zs)
	:x(x), y(y), z(z), xs(xs), ys(ys), zs(zs)
{

}

bool collide(AABB box, glm::vec3 point)
{
	return
		point.x >= box.x and point.x <= box.xs and
		point.y >= box.y and point.y <= box.ys and
		point.z >= box.z and point.z <= box.zs;
}