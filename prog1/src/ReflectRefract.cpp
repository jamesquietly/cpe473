#include "ReflectRefract.h"

glm::vec3 raytrace(glm::vec3 p0, glm::vec3 d, std::vector<GeomObj*> objList, std::vector<Light*> lightList, int depth) {
    glm::vec3 color, local, reflectColor, reflectionVec, intersectionPt, objNormal;
    glm::vec3 refractionVec, refractionColor;
    glm::vec4 objColor;
    color = glm::vec3(0, 0, 0);
    int hitNdx;
    float t, epsilon, DdotN, n1, n2, localContribut;
    float reflectCoeff, refractCoeff, filterVal;
    Ray ray = Ray(p0, d);
    epsilon = 0.001f;

    if (depth > 0) {

        hitNdx = first_hit(ray, objList, &t);
        if (hitNdx != -1) {
            local = blinn_phong(lightList, objList[hitNdx], ray, t, objList);
            intersectionPt = p0 + t * d;
            objNormal = objList[hitNdx]->get_normal(intersectionPt);
            reflectionVec = ray.calc_reflection(objNormal);
            reflectColor = raytrace(intersectionPt + reflectionVec * epsilon, reflectionVec, objList, lightList, depth - 1);
            reflectCoeff = (float)objList[hitNdx]->get_reflection();
            DdotN = glm::dot(d, objNormal);
            if (DdotN < 0) {
                n1 = 1.0f;
                n2 = objList[hitNdx]->get_ior();
            }
            else {
                n1 = objList[hitNdx]->get_ior();
                n2 = 1.0f;
                objNormal = -1.0f * objNormal;
            }
            refractionVec = ray.calc_refraction(objNormal, n1, n2);
            refractionColor = raytrace(intersectionPt + refractionVec * epsilon, refractionVec, objList, lightList, depth - 1);
            refractCoeff = (float)objList[hitNdx]->get_refraction();
            
            objColor = objList[hitNdx]->get_rgb();
            filterVal = objColor.w;
            localContribut = (1.0f - filterVal) * (1.0f - reflectCoeff);

            color += localContribut * local + reflectCoeff * reflectColor + refractCoeff * refractionColor;
        }
    }
    return color;
}