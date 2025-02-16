#ifndef __BVH_H__
#define __BVH_H__

#include "stdlib.h"
#include <vector>
#include <functional>
#include <algorithm>
#include "aabb.h"
#include "shape.h"

struct TimeAndShape {
    double time;
    Shape* shape;
    TimeAndShape(double t, Shape* s) : time(t), shape(s) {}
};

class BVHNode {
public:
    BVHNode* left;
    BVHNode* right;
    AABB bounds;
    Shape* shape;
    BVHNode(BVHNode* l, BVHNode* r, AABB b, Shape* s) : left(l), right(r), shape(s), bounds(b) {}

    BVHNode(std::vector<Shape*>& shapes, size_t start, size_t end);


    TimeAndShape getMinTimeAndShape(Ray ray);

    double getIntersection(Ray ray);

    bool getLightIntersection(Ray ray, double* fill); 

};
bool customCompare(Shape* a, Shape* b);

#endif