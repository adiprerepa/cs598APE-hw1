#include "bvh.h"

bool customCompare(Shape* a, Shape* b) {
    uint axis = 1;
    if (axis == 0) {
        double a_center = (a->bounds.min.x + a->bounds.max.x) * 0.5;
        double b_center = (b->bounds.min.x + b->bounds.max.x) * 0.5;
        return a_center < b_center;
    }
    if (axis == 1) {
        double a_center = (a->bounds.min.y + a->bounds.max.y) * 0.5;
        double b_center = (b->bounds.min.y + b->bounds.max.y) * 0.5;
        return a_center < b_center;
    }
    if (axis == 2) {
        double a_center = (a->bounds.min.z + a->bounds.max.z) * 0.5;
        double b_center = (b->bounds.min.z + b->bounds.max.z) * 0.5;
        return a_center < b_center;
    }
    return false;
}

BVHNode::BVHNode(std::vector<Shape*>& shapes, size_t start, size_t end): left(nullptr), right(nullptr), shape(nullptr), bounds(Vector(0, 0, 0), Vector(0, 0, 0)) {
    if (start == end) return;

    size_t objectSpan = end - start;
    if (objectSpan == 1) {
        // Only one shape: this node becomes a leaf.
        shape = shapes[start];
        bounds = shape->bounds;
    } else if (objectSpan == 2) {
        // Two shapes: sort them and assign.
        if (customCompare(shapes[start + 1], shapes[start]))
            std::swap(shapes[start], shapes[start + 1]);
        left = new BVHNode(nullptr, nullptr, shapes[start]->bounds, shapes[start]);
        right = new BVHNode(nullptr, nullptr, shapes[start + 1]->bounds, shapes[start + 1]);
        bounds = AABB(left->bounds, right->bounds);
    } else {
        // More than two shapes: sort and split.
        size_t mid = start + objectSpan / 2;
        left = new BVHNode(shapes, start, mid);
        right = new BVHNode(shapes, mid, end);
        bounds = AABB(left->bounds, right->bounds);
    }
}

TimeAndShape BVHNode::getMinTimeAndShape(Ray ray) {
    if (!bounds.intersects(ray))
        return TimeAndShape(inf, nullptr);
    if (shape) {
        double time = shape->getIntersection(ray);
        return TimeAndShape(time, time < inf ? shape : nullptr);
    }
    TimeAndShape leftTime = left ? left->getMinTimeAndShape(ray) : TimeAndShape(inf, nullptr);
    TimeAndShape rightTime = right ? right->getMinTimeAndShape(ray) : TimeAndShape(inf, nullptr);
    return leftTime.time < rightTime.time ? leftTime : rightTime;
}