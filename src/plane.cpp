#include "plane.h"

Plane::Plane(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty) : Shape(c, t, ya, pi, ro), vect(c), right(c), up(c){
   textureX = tx; textureY = ty;
   setAngles(yaw, pitch, roll);
   normalMap = NULL;
   mapX = textureX; mapY = textureY;
   bounds = AABB(Vector(-inf, -inf, -inf), Vector(inf, inf, inf));
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

   // Vector halfRight = right * (textureX / 2.0);
   //  Vector halfUp = up * (textureY / 2.0);

   //  Vector c1 = center + halfRight + halfUp;
   //  Vector c2 = center + halfRight - halfUp;
   //  Vector c3 = center - halfRight + halfUp;
   //  Vector c4 = center - halfRight - halfUp;

   //  // Calculate bounding box min/max values
   //  double minX = std::min(std::min(c1.x, c2.x), std::min(c3.x, c4.x));
   //  double minY = std::min(std::min(c1.y, c2.y), std::min(c3.y, c4.y));
   //  double minZ = std::min(std::min(c1.z, c2.z), std::min(c3.z, c4.z));
   //  double maxX = std::max(std::max(c1.x, c2.x), std::max(c3.x, c4.x));
   //  double maxY = std::max(std::max(c1.y, c2.y), std::max(c3.y, c4.y));
   //  double maxZ = std::max(std::max(c1.z, c2.z), std::max(c3.z, c4.z));

   //  bounds = AABB(Vector(minX, minY, minZ), Vector(maxX, maxY, maxZ));
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

   // Vector halfRight = right * (textureX / 2.0);
   //  Vector halfUp = up * (textureY / 2.0);

   //  Vector c1 = center + halfRight + halfUp;
   //  Vector c2 = center + halfRight - halfUp;
   //  Vector c3 = center - halfRight + halfUp;
   //  Vector c4 = center - halfRight - halfUp;

   //  // Calculate bounding box min/max values
   //  double minX = std::min(std::min(c1.x, c2.x), std::min(c3.x, c4.x));
   //  double minY = std::min(std::min(c1.y, c2.y), std::min(c3.y, c4.y));
   //  double minZ = std::min(std::min(c1.z, c2.z), std::min(c3.z, c4.z));
   //  double maxX = std::max(std::max(c1.x, c2.x), std::max(c3.x, c4.x));
   //  double maxY = std::max(std::max(c1.y, c2.y), std::max(c3.y, c4.y));
   //  double maxZ = std::max(std::max(c1.z, c2.z), std::max(c3.z, c4.z));

   //  bounds = AABB(Vector(minX, minY, minZ), Vector(maxX, maxY, maxZ));
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

   // Vector halfRight = right * (textureX / 2.0);
   //  Vector halfUp = up * (textureY / 2.0);

   //  Vector c1 = center + halfRight + halfUp;
   //  Vector c2 = center + halfRight - halfUp;
   //  Vector c3 = center - halfRight + halfUp;
   //  Vector c4 = center - halfRight - halfUp;

   //  // Calculate bounding box min/max values
   //  double minX = std::min(std::min(c1.x, c2.x), std::min(c3.x, c4.x));
   //  double minY = std::min(std::min(c1.y, c2.y), std::min(c3.y, c4.y));
   //  double minZ = std::min(std::min(c1.z, c2.z), std::min(c3.z, c4.z));
   //  double maxX = std::max(std::max(c1.x, c2.x), std::max(c3.x, c4.x));
   //  double maxY = std::max(std::max(c1.y, c2.y), std::max(c3.y, c4.y));
   //  double maxZ = std::max(std::max(c1.z, c2.z), std::max(c3.z, c4.z));

   //  bounds = AABB(Vector(minX, minY, minZ), Vector(maxX, maxY, maxZ));
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

   // Vector halfRight = right * (textureX / 2.0);
   //  Vector halfUp = up * (textureY / 2.0);

   //  Vector c1 = center + halfRight + halfUp;
   //  Vector c2 = center + halfRight - halfUp;
   //  Vector c3 = center - halfRight + halfUp;
   //  Vector c4 = center - halfRight - halfUp;

   //  // Calculate bounding box min/max values
   //  double minX = std::min(std::min(c1.x, c2.x), std::min(c3.x, c4.x));
   //  double minY = std::min(std::min(c1.y, c2.y), std::min(c3.y, c4.y));
   //  double minZ = std::min(std::min(c1.z, c2.z), std::min(c3.z, c4.z));
   //  double maxX = std::max(std::max(c1.x, c2.x), std::max(c3.x, c4.x));
   //  double maxY = std::max(std::max(c1.y, c2.y), std::max(c3.y, c4.y));
   //  double maxZ = std::max(std::max(c1.z, c2.z), std::max(c3.z, c4.z));

   //  bounds = AABB(Vector(minX, minY, minZ), Vector(maxX, maxY, maxZ));
}

double Plane::getIntersection(Ray ray) {
    const double t = ray.vector.x * vect.x + ray.vector.y * vect.y + ray.vector.z * vect.z;
    if (t == 0.0) return inf;
    const double norm = ray.point.x * vect.x + ray.point.y * vect.y + ray.point.z * vect.z + d;
    const double r = -norm/t;
    return r > 0.0 ? r : inf;
}

Vector Plane::solveScalers(Vector C) {
    const double inv_denom = 1.0 / denom;
    const double Cx = C.x, Cy = C.y, Cz = C.z;
    
    const double Cz_v2y = Cz * v2yv3x;
    const double Cy_v2z = Cy * v2zv3x;
    const double Cz_v2x = Cz * v2xv3y;
    const double Cx_v2z = Cx * v2zv3y;
    const double Cy_v2x = Cy * v2xv3z;
    const double Cx_v2y = Cx * v2yv3z;
    
    const double a = (Cz_v2y - Cy_v2z - Cz_v2x + Cx_v2z + Cy_v2x - Cx_v2y) * inv_denom;
    const double b = (-Cz * v1yv3x + Cy * v1zv3x + Cz * v1xv3y - Cx * v1zv3y - Cy * v1xv3z + Cx * v1yv3z) * inv_denom;
    const double c = (Cz * v1yv2x - Cy * v1zv2x - Cz * v1xv2y + Cx * v1zv2y + Cy * v1xv2z - Cx * v1yv2z) * inv_denom;
    
    return Vector(a, b, c);
}

bool Plane::getLightIntersection(Ray ray, double* fill) {
    const double t = ray.vector.dot(vect);
    if (t == 0.0) return false;  // avoid division by zero
    
    const double norm = vect.dot(ray.point) + d;
    const double r = -norm/t;
    if (r <= 0.0 || r >= 1.0) return false;
    
    if (texture->opacity > 1.0 - 1E-6) return true;
    
    const Vector& dist = solveScalers(ray.point - center);
    const double tx = fix(dist.x/textureX - 0.5);
    const double ty = fix(dist.y/textureY - 0.5);
    
    unsigned char temp[4];
    double amb, op, ref;
    texture->getColor(temp, &amb, &op, &ref, tx, ty);
    
    if (op > 1.0 - 1E-6) return true;
    
    fill[0] *= temp[0]/255.0;
    fill[1] *= temp[1]/255.0;
    fill[2] *= temp[2]/255.0;
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