#include "aabb.h"

AABB::AABB(Vector min, Vector max) : min(min), max(max) {}

AABB::AABB(AABB a, AABB b) : max(Vector(0, 0, 0)), min(Vector(0, 0, 0)) {
    min.x = std::min(a.min.x, b.min.x);
    min.y = std::min(a.min.y, b.min.y);
    min.z = std::min(a.min.z, b.min.z);
    max.x = std::max(a.max.x, b.max.x);
    max.y = std::max(a.max.y, b.max.y);
    max.z = std::max(a.max.z, b.max.z);
}

bool AABB::intersects(Ray ray) {
    double tmin = (min.x - ray.point.x) / ray.vector.x;
    double tmax = (max.x - ray.point.x) / ray.vector.x;

    if (tmin > tmax) std::swap(tmin, tmax);

    double tymin = (min.y - ray.point.y) / ray.vector.y;
    double tymax = (max.y - ray.point.y) / ray.vector.y;

    if (tymin > tymax) std::swap(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax)) return false;

    if (tymin > tmin) tmin = tymin;
    if (tymax < tmax) tmax = tymax;

    double tzmin = (min.z - ray.point.z) / ray.vector.z;
    double tzmax = (max.z - ray.point.z) / ray.vector.z;

    if (tzmin > tzmax) std::swap(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax)) return false;

    return true;
}