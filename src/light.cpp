
#include "light.h"
#include "shape.h"
#include "camera.h"
      
Light::Light(const Vector & cente, unsigned char* colo) : center(cente){
   color = colo;
}

unsigned char* Light::getColor(unsigned char a, unsigned char b, unsigned char c){
   unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*3);
   r[0] = a;
   r[1] = b;
   r[2] = c;
   return r;
}

Autonoma::Autonoma(const Camera& c): camera(c){
   listStart = NULL;
   listEnd = NULL;
   lightStart = NULL;
   lightEnd = NULL;
   depth = 10;
   skybox = BLACK;
}

Autonoma::Autonoma(const Camera& c, Texture* tex): camera(c){
   listStart = NULL;
   listEnd = NULL;
   lightStart = NULL;
   lightEnd = NULL;
   depth = 10;
   skybox = tex;
}

void Autonoma::addShape(Shape* r){
   ShapeNode* hi = (ShapeNode*)malloc(sizeof(ShapeNode));
   hi->data = r;
   hi->next = hi->prev = NULL;
   if(listStart==NULL){
      listStart = listEnd = hi;
   }
   else{
      listEnd->next = hi;
      hi->prev = listEnd;
      listEnd = hi;
   }
}

void Autonoma::removeShape(ShapeNode* s){
   if(s==listStart){
      if(s==listEnd){
         listStart = listStart = NULL;
      }
      else{
         listStart = s->next;
         listStart->prev = NULL;
      }
   }
   else if(s==listEnd){
      listEnd = s->prev;
      listEnd->next = NULL;
   }
   else{
      ShapeNode *b4 = s->prev, *aft = s->next;
      b4->next = aft;
      aft->prev = b4;
   }
   free(s);
}

void Autonoma::addLight(Light* r){
   LightNode* hi = (LightNode*)malloc(sizeof(LightNode));
   hi->data = r;
   hi->next = hi->prev = NULL;
   if(lightStart==NULL){
      lightStart = lightEnd = hi;
   }
   else{
      lightEnd->next = hi;
      hi->prev = lightEnd;
      lightEnd = hi;
   }
}

void Autonoma::removeLight(LightNode* s){
   if(s==lightStart){
      if(s==lightEnd){
         lightStart = lightStart = NULL;
      }
      else{
         lightStart = s->next;
         lightStart->prev = NULL;
      }
   }
   else if(s==lightEnd){
      lightEnd = s->prev;
      lightEnd->next = NULL;
   }
   else{
      LightNode *b4 = s->prev, *aft = s->next;
      b4->next = aft;
      aft->prev = b4;
   }
   free(s);
}

void getLight(double* tColor, Autonoma* aut, Vector point, Vector norm, unsigned char flip) {
   tColor[0] = tColor[1] = tColor[2] = 0.;
   
   // Pre-compute norm magnitude once since it's used repeatedly
   const double normMag = norm.mag();
   if(normMag < 1e-6) return; // Early exit if normal is too small
   const double normMagInv = 1.0/normMag; // Precompute inverse for division

   for(LightNode* t = aut->lightStart; t != NULL; t = t->next) {
      // Calculate light ray vector first to enable early rejection
      Vector ra = t->data->center-point;
      const double raMag = ra.mag();
      if(raMag < 1e-6) continue; // Skip if light source too close
      const double raMagInv = 1.0/raMag; // Precompute inverse

      // Calculate dot product early for potential early rejection
      double perc = norm.dot(ra) * (raMagInv * normMagInv); // Combined division
      if(flip && perc < 0) perc = -perc;
      if(perc <= 0) continue; // Skip if light is behind surface

      // Convert colors to doubles once
      const double r = t->data->color[0]/255.;
      const double g = t->data->color[1]/255.; 
      const double b = t->data->color[2]/255.;
      double lightColor[3] = {r, g, b};

      // Check for occlusion with early exit
      bool hit = false;
      const Ray shadowRay(point+ra*0.01, ra);
      for(ShapeNode* shapeIter = aut->listStart; !hit && shapeIter != NULL; shapeIter = shapeIter->next) {
         hit = shapeIter->data->getLightIntersection(shadowRay, lightColor);
      }
      
      if(!hit) {
         // Add light contribution with combined multiply
         tColor[0] = fmin(1.0, tColor[0] + perc*lightColor[0]);
         tColor[1] = fmin(1.0, tColor[1] + perc*lightColor[1]); 
         tColor[2] = fmin(1.0, tColor[2] + perc*lightColor[2]);
      }
   }
}
