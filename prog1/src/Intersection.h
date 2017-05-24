#ifndef __Intersection__
#define __Intersection__

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Ray.h"

class Intersection {
public:
    Intersection();
    Intersection(Ray r, Ray transfRay, float tVal, int hNdx);
    Ray get_ray() const {return ray;}
    Ray get_transformedRay() const {return transformedRay;}
    float get_t() const {return t;}
    int get_hitNdx() const {return hitNdx;}

private:
    Ray ray, transformedRay;
    float t;
    int hitNdx;
};

#endif