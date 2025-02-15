#ifndef __PLANE_H__
#define __PLANE_H__

#include "shape.h"

class Plane : public Shape{
public:
  Vector vect, right, up;
  double denom;

  double v1xv2y, v1xv2z, v1xv3y, v1xv3z;
  double v1yv2x, v1yv2z, v1yv3x, v1yv3z;
  double v1zv2x, v1zv2y, v1zv3x, v1zv3y;

  double v2xv3y, v2xv3z;
  double v2yv3x, v2yv3z;
  double v2zv3x, v2zv3y;


  double d;
  Plane(const Vector &c, Texture* t, double ya, double pi, double ro, double tx, double ty);
  double getIntersection(Ray ray);
  bool getLightIntersection(Ray ray, double* toFill);
  void move();
  void getColor(unsigned char* toFill, double* am, double* op, double* ref, Autonoma* r, Ray ray, unsigned int depth);
  Vector getNormal(Vector point);
  unsigned char reversible();
  void setAngles(double yaw, double pitch, double roll);
  void setYaw(double d);
  void setPitch(double d);
  void setRoll(double d);
  Vector solveScalers(Vector C);
};

#endif
