#ifndef __ReflectRefract__
#define __ReflectRefract__

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "GeomObj.h"
#include "Light.h"
#include "Ray.h"
#include "Shading.h"

float schlick_approx(float ior, glm::vec3 normal, glm::vec3 view);
glm::vec3 beers_law(glm::vec4 objColor, float distance);
glm::vec3 raytrace(glm::vec3 p0, glm::vec3 d, std::vector<GeomObj*> objList, std::vector<Light*> lightList, int depth);

#endif