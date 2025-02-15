#include "triangle.h"

Triangle::Triangle(Vector c, Vector b, Vector a, Texture* t):Plane(Vector(0,0,0), t, 0., 0., 0., 0., 0.){
   center = c;
   Vector righta = (b-c);
   textureX = righta.mag();
   right = righta/textureX;
   vect = right.cross(b-a).normalize();

   xsin = -right.z;
   if(xsin<-1.)xsin = -1;
   else if (xsin>1.)xsin=1.; 
   yaw = asin(xsin);
   xcos = sqrt(1.-xsin*xsin);

   zcos = right.x/xcos;
   zsin = -right.y/xcos;
   if(zsin<-1.)zsin = -1;
   else if (zsin>1.)zsin=1.;
   if(zcos<-1.)zcos = -1;
   else if (zcos>1.)zcos=1.;
   roll = asin(zsin);

   ycos = vect.z/xcos;
   if(ycos<-1.)ycos = -1;
   else if (ycos>1.)ycos=1.;
   pitch = acos(ycos);
   ysin = sqrt(1-ycos*ycos);

   up.x = -xsin*ysin*zcos+ycos*zsin;
   up.y = ycos*zcos+xsin*ysin*zsin;
   up.z = -xcos*ysin;
   Vector temp = vect.cross(right);
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
   Vector np = solveScalers(a-c);
   textureY = np.y;
   thirdX = np.x;

   d = -vect.dot(center);
}

double Triangle::getIntersection(Ray ray){
   double time = Plane::getIntersection(ray);
   if(time==inf) 
      return time;
   Vector dist = solveScalers(ray.point+ray.vector*time-center); 
   unsigned char tmp = (thirdX - dist.x) * textureY + (thirdX-textureX) * (dist.y - textureY) < 0.0;
   return((tmp!=(textureX * dist.y < 0.0)) || (tmp != (dist.x * textureY - thirdX * dist.y < 0.0)))?inf:time;
}

bool Triangle::getLightIntersection(Ray ray, double* fill){
   const double t = ray.vector.dot(vect);
   const double norm = vect.dot(ray.point)+d;
   const double r = -norm/t;
   if(r<=0. || r>=1.) return false;
   Vector dist = solveScalers(ray.point+ray.vector*r-center);
   
   unsigned char tmp = (thirdX - dist.x) * textureY + (thirdX-textureX) * (dist.y - textureY) < 0.0;
   if ((tmp!=(textureX * dist.y < 0.0)) || (tmp != (dist.x * textureY - thirdX * dist.y < 0.0))) return false;
   
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
