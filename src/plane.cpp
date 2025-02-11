#include "plane.h"

Plane::Plane(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty) : Shape(c, t, ya, pi, ro), vect(c), right(c), up(c){
   textureX = tx; textureY = ty;
   setAngles(yaw, pitch, roll);
   normalMap = NULL;
   mapX = textureX; mapY = textureY;
}

void Plane::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   right.z = -xsin;
   denom = right.z * up.y * vect.x - right.y * up.z * vect.x
                 - right.z * up.x * vect.y + right.x * up.z * vect.y
                 + right.y * up.x * vect.z - right.x * up.y * vect.z;
   
   // right components multiplied with up and vect components
   v1xv2y = right.x * up.y;
   v1xv2z = right.x * up.z;
   v1xv3y = right.x * vect.y;
   v1xv3z = right.x * vect.z;
   
   v1yv2x = right.y * up.x;
   v1yv2z = right.y * up.z;
   v1yv3x = right.y * vect.x;
   v1yv3z = right.y * vect.z;
   
   v1zv2x = right.z * up.x;
   v1zv2y = right.z * up.y;
   v1zv3x = right.z * vect.x;
   v1zv3y = right.z * vect.y;
   
   // up components multiplied with vect components
   v2xv3y = up.x * vect.y;
   v2xv3z = up.x * vect.z;
   
   v2yv3x = up.y * vect.x;
   v2yv3z = up.y * vect.z;
   
   v2zv3x = up.z * vect.x;
   v2zv3y = up.z * vect.y;

   d = -vect.dot(center);
}

void Plane::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
   
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;   
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   right.z = -xsin;
   d = -vect.dot(center);
}

void Plane::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   d = -vect.dot(center);
}

void Plane::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
   vect.x = xsin*ycos*zcos+ysin*zsin;
   vect.y = ysin*zcos-xsin*ycos*zsin;
//   vect.z = xcos*ycos;
   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
  // up.z = -xcos*ysin;
   right.x = xcos*zcos;
   right.y = -xcos*zsin;
   //right.z = -xsin;
   d = -vect.dot(center);
}

double Plane::getIntersection(Ray ray){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   return (r>0)?r:inf;
}

Vector Plane::solveScalers(Vector C) {
   //  double denom = v1z * v2y * vect.x - v1y * v2z * vect.x
               //   - v1z * v2x * v3y + v1x * v2z * v3y
               //   + v1y * v2x * v3z - v1x * v2y * v3z;

    double a = C.z * v2yv3x - C.y * v2zv3x
             - C.z * v2xv3y + C.x * v2zv3y
             + C.y * v2xv3z - C.x * v2yv3z;

    double b = -C.z * v1yv3x + C.y * v1zv3x
              + C.z * v1xv3y - C.x * v1zv3y
              - C.y * v1xv3z + C.x * v1yv3z;

    double c = C.z * v1yv2x - C.y * v1zv2x
             - C.z * v1xv2y + C.x * v1zv2y
             + C.y * v1xv2z - C.x * v1yv2z;

   return Vector(a / denom, b / denom, c / denom);
}

bool Plane::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;

   if(texture->opacity>1-1E-6) return true;   
   Vector dist = solveScalers(ray.point-center);
   unsigned char temp[4];
   double amb, op, ref;
   texture->getColor(temp, &amb, &op, &ref,fix(dist.x/textureX-.5), fix(dist.y/textureY-.5));
   if(op>1-1E-6) return true;
   fill[0]*=temp[0]/255.;
   fill[1]*=temp[1]/255.;
   fill[2]*=temp[2]/255.;
   return false;
}

void Plane::move(){
   d = -vect.dot(center);
}
void Plane::getColor(unsigned char* toFill,double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth){
   Vector dist = solveScalers(ray.point-center);
   texture->getColor(toFill, am, op, ref, fix(dist.x/textureX-.5), fix(dist.y/textureY-.5));
}
unsigned char Plane::reversible(){ 
   return 1; }

Vector Plane::getNormal(Vector point){
   if(normalMap==NULL)
      return vect;
   else{
      Vector dist = solveScalers(point-center);
      double am, ref, op;
      unsigned char norm[3];
      normalMap->getColor(norm, &am, &op, &ref, fix(dist.x/mapX-.5+mapOffX), fix(dist.y/mapY-.5+mapOffY));
      Vector ret = ((norm[0]-128)*right+(norm[1]-128)*up+norm[2]*vect).normalize();
      return ret;
   }
}