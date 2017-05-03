#ifndef __Shading__
#define __Shading__
#include <vector>
#include "Light.h"
#include "GeomObj.h"
#include "Ray.h"

glm::vec3 blinn_phong(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, std::vector<GeomObj*> objList);

glm::vec3 ck_diff_spec(float d, float s, float Rd, float roughness, float ior, glm::vec3 normal, glm::vec3 H, glm::vec3 V, glm::vec3 lightDir, glm::vec3 lightColor, glm::vec3 objColor);

glm::vec3 cook_torrance(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, std::vector<GeomObj*> objList);

#endif