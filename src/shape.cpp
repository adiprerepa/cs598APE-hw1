#include "shape.h"

Shape::Shape(const Vector &c, Texture* t, double ya, double pi, double ro): center(c), texture(t), yaw(ya), pitch(pi), roll(ro){
};

void Shape::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
}

void Shape::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
}

void Shape::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
}

void Shape::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
}

typedef struct {
   double time;
   Shape* shape;
} TimeAndShape;

void insertionSort(TimeAndShape *arr, int n) {
    for (int i = 1; i < n; ++i) {
        TimeAndShape key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].time > key.time) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void calcColor(unsigned char* toFill, Autonoma* c, Ray ray, unsigned int depth){
    // Find the single closest intersection
    ShapeNode* t = c->listStart;
    double minTime = inf;
    Shape* minShape = nullptr;
    while (t != NULL) {
        double time = t->data->getIntersection(ray);
        if (time < minTime) {
            minTime = time;
            minShape = t->data;
        }
        t = t->next;
    }

    // If no intersection or time is infinite, use the skybox
    if (minShape == nullptr || minTime == inf) {
        double opacity, reflection, ambient;
        // Normalize and compute skybox texture coords
        Vector temp = ray.vector.normalize();
        double me = fabs(temp.y);
        double angle = atan2(temp.z, temp.x);

        // Assuming fix(...) is needed to wrap or clamp the coords
        c->skybox->getColor(toFill, &ambient, &opacity, &reflection, 
                            fix(angle / M_TWO_PI), fix(me));
        return;
    }

    // We have a nearest shape intersection
    Vector intersect = ray.point + ray.vector * minTime;
    double opacity, reflection, ambient;
    minShape->getColor(toFill, &ambient, &opacity, &reflection, c, Ray(intersect, ray.vector), depth);

    // Compute lighting
    double lightData[3];
    Vector rawNormal = minShape->getNormal(intersect);
    getLight(lightData, c, intersect, rawNormal, minShape->reversible());

    // Combine ambient + light color
    toFill[0] = (unsigned char)(toFill[0] * (ambient + lightData[0]*(1 - ambient)));
    toFill[1] = (unsigned char)(toFill[1] * (ambient + lightData[1]*(1 - ambient)));
    toFill[2] = (unsigned char)(toFill[2] * (ambient + lightData[2]*(1 - ambient)));

    // Handle opacity (transparency) and reflection, recursively
    if (depth < c->depth && (opacity < 1.0 - 1e-6 || reflection > 1e-6)) {
        unsigned char col[4];

        // Transparency
        if (opacity < 1.0 - 1e-6) {
            Ray nextRay(intersect + ray.vector * 1e-4, ray.vector);
            calcColor(col, c, nextRay, depth + 1);
            toFill[0] = (unsigned char)(toFill[0] * opacity + col[0] * (1 - opacity));
            toFill[1] = (unsigned char)(toFill[1] * opacity + col[1] * (1 - opacity));
            toFill[2] = (unsigned char)(toFill[2] * opacity + col[2] * (1 - opacity));
        }

        // Reflection
        if (reflection > 1e-6) {
            // Use normalized normal for reflection bounce
            Vector nrm = rawNormal.normalize();
            double dotVal = nrm.dot(ray.vector);
            Vector reflectVec = ray.vector - (2.0 * dotVal) * nrm;
            Ray nextRay(intersect + reflectVec * 1e-4, reflectVec);

            calcColor(col, c, nextRay, depth + 1);
            toFill[0] = (unsigned char)(toFill[0] * (1 - reflection) + col[0] * reflection);
            toFill[1] = (unsigned char)(toFill[1] * (1 - reflection) + col[1] * reflection);
            toFill[2] = (unsigned char)(toFill[2] * (1 - reflection) + col[2] * reflection);
        }
    }
}
