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
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*time-center);
   if(time==inf) 
      return time;
   return ( ((dist.x>=0)?dist.x:-dist.x)>textureX/2 || ((dist.y>=0)?dist.y:-dist.y)>textureY/2 )?inf:time;
}

bool Box::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*r-center);
   if( ((dist.x>=0)?dist.x:-dist.x)>textureX/2 || ((dist.y>=0)?dist.y:-dist.y)>textureY/2 ) return false;

   if(texture->opacity>1-1E-6) return true;   
   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dist.x/textureX-.5), fix(dist.y/textureY-.5));
   if(op>1-1E-6) return true;
   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}