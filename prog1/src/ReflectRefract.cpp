#include "ReflectRefract.h"

float schlicks_aprrox(float ior, glm::vec3 normal, glm::vec3 view) {
    float F0, F, NdotV;
    F0 = glm::pow(ior - 1.0f, 2.0f)/glm::pow(ior + 1.0f, 2.0f);
    NdotV = glm::dot(normal, view);
    F = F0 + (1.0f - F0) * glm::pow(1.0f - NdotV, 5.0f);
    return F;
}

glm::vec3 beers_law(glm::vec4 objColor, float distance) {
    float alpha;
    glm::vec3 color, absorbance, attenuation;
    color = glm::vec3(objColor.x, objColor.y, objColor.z);
    alpha = 0.15f;
    absorbance = (glm::vec3(1.0f, 1.0f, 1.0f) - color) * alpha * -1.0f * distance;
    attenuation = glm::vec3(glm::exp(absorbance.x), glm::exp(absorbance.y), glm::exp(absorbance.z));
    return attenuation;
}

glm::vec3 raytrace(glm::vec3 p0, glm::vec3 d, std::vector<GeomObj*> objList, std::vector<Light*> lightList, int depth, bool printMode) {
    glm::vec3 color, local, reflectColor, reflectionVec, intersectionPt, objNormal;
    glm::vec3 refractionVec, refractionColor, attenuation;
    glm::vec4 objColor;
    color = glm::vec3(0, 0, 0);
    int hitNdx;
    float t, epsilon, DdotN, n1, n2, localContribut, reflectContribut, refractContribut;
    float reflectCoeff, refractCoeff, filterVal, fresnelReflectance, ior, distance;
    Ray ray = Ray(p0, d);
    epsilon = 0.001f;

    if (depth > 0) {

        hitNdx = first_hit(ray, objList, &t);
        if (hitNdx != -1) {
            intersectionPt = p0 + t * d;
            objNormal = objList[hitNdx]->get_normal(intersectionPt);
            reflectionVec = ray.calc_reflection(objNormal);
            reflectColor = raytrace(intersectionPt + reflectionVec * epsilon, reflectionVec, objList, lightList, depth - 1, printMode);
            reflectCoeff = (float)objList[hitNdx]->get_reflection();
            DdotN = glm::dot(d, objNormal);
            ior = (float)objList[hitNdx]->get_ior();
            if (DdotN < 0) {
                n1 = 1.0f;
                n2 = ior;
            }
            else {
                n1 = ior;
                n2 = 1.0f;
                objNormal = -1.0f * objNormal;
            }
            refractionVec = ray.calc_refraction(objNormal, n1, n2);
            refractionColor = raytrace(intersectionPt + refractionVec * epsilon, refractionVec, objList, lightList, depth - 1, printMode);
            refractCoeff = (float)objList[hitNdx]->get_refraction();
            
            objColor = objList[hitNdx]->get_rgb();
            filterVal = objColor.w;
            fresnelReflectance = schlicks_aprrox(ior, objNormal, -1.0f * d);
            localContribut = (1.0f - filterVal) * (1.0f - reflectCoeff);
            reflectContribut = (1.0f - filterVal) * reflectCoeff + filterVal * fresnelReflectance;
            refractContribut = filterVal * (1.0f - fresnelReflectance);
            distance = glm::distance(p0, intersectionPt);
            attenuation = beers_law(objColor, distance);

            if (printMode) {
                std::cout << "Ray: {" << p0.x << " " << p0.y << " " << p0.z << "} -> {";
                std::cout << d.x << " " << d.y << " " << d.z << "}\n";
                std::cout << "Hit Object ID (" << hitNdx << " - " << objList[hitNdx]->get_type() << ") at T = ";
                std::cout << t << ", Intersection = {" << intersectionPt.x << " " << intersectionPt.y << " " << intersectionPt.z << "}\n";
                std::cout << "Normal {" << objNormal.x << " " << objNormal.y << " " << objNormal.z << "}\n";
            }

            local = blinn_phong(lightList, objList[hitNdx], ray, t, objList, printMode);


            color += (localContribut * local + reflectContribut * reflectColor + refractContribut * refractionColor);
        }
    }
    return color;
}