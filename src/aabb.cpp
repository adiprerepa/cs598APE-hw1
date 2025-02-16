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

bool AABB::intersects(const Ray &ray) const {
    // Precompute inverse direction components.
    double invDirX = 1.0 / ray.vector.x;
    double invDirY = 1.0 / ray.vector.y;
    double invDirZ = 1.0 / ray.vector.z;

    // X slab
    double tmin = (min.x - ray.point.x) * invDirX;
    double tmax = (max.x - ray.point.x) * invDirX;
    if (invDirX < 0.0)
        std::swap(tmin, tmax);

    // Y slab
    double tymin = (min.y - ray.point.y) * invDirY;
    double tymax = (max.y - ray.point.y) * invDirY;
    if (invDirY < 0.0)
        std::swap(tymin, tymax);

    // Check for overlap between X and Y slabs.
    if ((tmin > tymax) || (tymin > tmax))
        return false;
    
    // Merge the slabs.
    if (tymin > tmin)
        tmin = tymin;
    if (tymax < tmax)
        tmax = tymax;

    // Z slab
    double tzmin = (min.z - ray.point.z) * invDirZ;
    double tzmax = (max.z - ray.point.z) * invDirZ;
    if (invDirZ < 0.0)
        std::swap(tzmin, tzmax);

    // Final overlap test with Z slab.
    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    return true;
}
