#include "box.h"
#include <algorithm> // for std::min and std::max

Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty)
    : Plane(c, t, ya, pi, ro, tx, ty)
{
    // Compute half extents along right and up vectors.
    Vector halfRight = right * (textureX / 2.0);
    Vector halfUp = up * (textureY / 2.0);

    // Compute the four corners of the box.
    Vector c1 = center + halfRight + halfUp;
    Vector c2 = center + halfRight - halfUp;
    Vector c3 = center - halfRight + halfUp;
    Vector c4 = center - halfRight - halfUp;

    // Calculate min/max coordinates with nested std::min and std::max.
    double minX = std::min(std::min(c1.x, c2.x), std::min(c3.x, c4.x));
    double minY = std::min(std::min(c1.y, c2.y), std::min(c3.y, c4.y));
    double minZ = std::min(std::min(c1.z, c2.z), std::min(c3.z, c4.z));
    double maxX = std::max(std::max(c1.x, c2.x), std::max(c3.x, c4.x));
    double maxY = std::max(std::max(c1.y, c2.y), std::max(c3.y, c4.y));
    double maxZ = std::max(std::max(c1.z, c2.z), std::max(c3.z, c4.z));

    // Set the bounding box.
    bounds = AABB(Vector(minX, minY, minZ), Vector(maxX, maxY, maxZ));
}

Box::Box(const Vector &c, Texture* t, double ya, double pi, double ro, double tx)
    : Plane(c, t, ya, pi, ro, tx, tx)
{
    Vector halfRight = right * (textureX / 2.0);
    Vector halfUp = up * (textureY / 2.0);

    Vector c1 = center + halfRight + halfUp;
    Vector c2 = center + halfRight - halfUp;
    Vector c3 = center - halfRight + halfUp;
    Vector c4 = center - halfRight - halfUp;

    double minX = std::min(std::min(c1.x, c2.x), std::min(c3.x, c4.x));
    double minY = std::min(std::min(c1.y, c2.y), std::min(c3.y, c4.y));
    double minZ = std::min(std::min(c1.z, c2.z), std::min(c3.z, c4.z));
    double maxX = std::max(std::max(c1.x, c2.x), std::max(c3.x, c4.x));
    double maxY = std::max(std::max(c1.y, c2.y), std::max(c3.y, c4.y));
    double maxZ = std::max(std::max(c1.z, c2.z), std::max(c3.z, c4.z));

    bounds = AABB(Vector(minX, minY, minZ), Vector(maxX, maxY, maxZ));
}


double Box::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;
   Vector dist = solveScalers(ray.point+ray.vector*time-center);

   return ( (std::abs(dist.x) * 2) > textureX || (std::abs(dist.y) * 2 ) > textureY ) ? inf : time;
}

bool Box::getLightIntersection(Ray ray, double* fill) {
    const double t = ray.vector.dot(vect);
    if (std::abs(t) < 1e-10) return false;  // avoid division by near-zero
    
    const double norm = vect.dot(ray.point) + d;
    const double r = -norm/t;
    if (r <= 0. || r >= 1.) return false;
    
    const Vector intersection_point = ray.point + ray.vector * r;
    const Vector dist = solveScalers(intersection_point - center);
    
    const double abs_dx = std::abs(dist.x);
    const double abs_dy = std::abs(dist.y);
    if (abs_dx > textureX/2 || abs_dy > textureY/2) return false;
    
    if (texture->opacity > 1-1E-6) return true;
    
    const double tex_x = dist.x/textureX - 0.5;
    const double tex_y = dist.y/textureY - 0.5;
    
    unsigned char temp[4];
    double amb, op, ref;
    texture->getColor(temp, &amb, &op, &ref, fix(tex_x), fix(tex_y));
    
    if (op > 1-1E-6) return true;
    
    fill[0] *= temp[0]/255.;
    fill[1] *= temp[1]/255.;
    fill[2] *= temp[2]/255.;
    return false;
}