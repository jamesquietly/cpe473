#ifndef __Shading__
#define __Shading__
#include <vector>
#include <iostream>
#include <cmath>
#include "Light.h"
#include "GeomObj.h"
#include "Ray.h"
#include "Intersection.h"

bool shadow_check();
glm::vec3 nan_check(glm::vec3 v);

glm::vec3 blinn_phong(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, glm::vec3 objNormal, std::vector<GeomObj*> objList, bool printMode);
glm::vec3 blinn_phong_diffspec(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, glm::vec3 objNormal, std::vector<GeomObj*> objList, bool printMode);
glm::vec3 blinn_phong_soft_shadow(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, glm::vec3 objNormal, std::vector<GeomObj*> objList, bool printMode);

glm::vec3 ck_diff_spec(float d, float s, float Rd, float roughness, float ior, glm::vec3 normal, glm::vec3 H, glm::vec3 V, glm::vec3 lightDir, glm::vec3 lightColor, glm::vec3 objColor);

glm::vec3 cook_torrance(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, std::vector<GeomObj*> objList, bool printMode);

#endif