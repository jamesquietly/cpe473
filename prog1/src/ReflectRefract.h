#ifndef __ReflectRefract__
#define __ReflectRefract__

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include "GeomObj.h"
#include "Light.h"
#include "Ray.h"
#include "Shading.h"
#include "Intersection.h"

class OptionalArgs {
public:
    OptionalArgs();
    OptionalArgs(bool print, bool alt, bool fresnel, bool gi, bool soft, int numSamples, int nBounces);
    bool get_printMode() const {return printMode;}
    bool get_altBRDF() const {return altBRDF;}
    bool get_useFresnel() const {return useFresnel;}
    bool get_useGI() const {return useGI;}
    bool get_useSoftShadow() const {return useSoftShadow;}
    int get_numGISamples() {return numGISamples;}
    int get_numBounces() {return numBounces;}

    void set_numGISamples(int n) {numGISamples = n;}
    void set_numBounces(int n) {numBounces = n;}
    void set_printMode(bool b) {printMode = b;}
private:
    bool printMode, altBRDF, useFresnel, useGI, useSoftShadow;
    int numGISamples, numBounces;
};

float schlick_approx(float ior, glm::vec3 normal, glm::vec3 view);
glm::vec3 beers_law(glm::vec4 objColor, float distance);
glm::vec3 cosineWeightedPoint();
float frand();
glm::vec3 uniformSamplePoint(float u, float v);
glm::vec3 alignSampleVector(glm::vec3 sample, glm::vec3 up, glm::vec3 normal);
std::vector<glm::vec3> generate_hemisphere_sample_pts(int numSamples, glm::vec3 normal);
std::vector<glm::vec3> generate_uniform_sample_pts(int numSamples, glm::vec3 normal);
glm::vec3 raytrace(glm::vec3 p0, glm::vec3 d, float *tPassBack, std::vector<GeomObj*> objList, std::vector<Light*> lightList, int depth, std::string rayType, OptionalArgs optObj);

#endif