#include "Intersection.h"

Intersection::Intersection() {
    ray = Ray();
    transformedRay = Ray();
    t = 0;
    hitNdx = -1;
}

Intersection::Intersection(Ray r, Ray transfRay, float tVal, int hNdx) {
    ray = Ray(r.get_pt(), r.get_direction());
    transformedRay = Ray(transfRay.get_pt(), transfRay.get_direction());
    t = tVal;
    hitNdx = hNdx;
}
