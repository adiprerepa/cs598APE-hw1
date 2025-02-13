#include "disk.h"
Disk::Disk(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty):Plane(c, t, ya, pi, ro, tx, ty){}


double Disk::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*time-center);
   double dx2 = dist.x*dist.x;
   double dy2 = dist.y*dist.y;
   double tx2 = textureX*textureX;
   double ty2 = textureY*textureY;
   return (ty2 * dx2 + dy2 * tx2 > tx2 * ty2)?inf:time;
}

bool Disk::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   bool sameSign = ((*(unsigned long long*)&norm ^ *(unsigned long long*)&t) & 0x8000000000000000ULL) == 0;
   if (norm == 0. || (t > 0. && (sameSign || norm >= -t)) || (t < 0. && (!sameSign || norm <= -t))) return false;
   const double r = -norm/t;
   Vector dist = solveScalers(right, up, vect, ray.point+ray.vector*r-center);
   double dx2 = dist.x*dist.x;
   double dy2 = dist.y*dist.y;
   double tx2 = textureX*textureX;
   double ty2 = textureY*textureY;
   if(ty2 * dx2 + dy2 * tx2 > tx2 * ty2)return false;
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