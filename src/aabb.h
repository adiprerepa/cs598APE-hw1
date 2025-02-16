#ifndef __AABB_H__
#define __AABB_H__
#include "vector.h"
#include <vector>

class AABB {
public:
    AABB(AABB a, AABB b);
    AABB(Vector min, Vector max);
    Vector min;
    Vector max;
    bool intersects(Ray ray);
};

#endif
