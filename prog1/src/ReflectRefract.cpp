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

glm::vec3 raytrace(glm::vec3 p0, glm::vec3 d, float *tPassBack, std::vector<GeomObj*> objList, std::vector<Light*> lightList, int depth, bool printMode, std::string rayType, bool altBRDF) {
    glm::vec3 color, local, reflectColor, reflectionVec, intersectionPt, objNormal;
    glm::vec3 refractionVec, refractionColor, attenuation, refractPt;
    glm::vec4 objColor;
    bool enterMedia = false;
    color = glm::vec3(0, 0, 0);
    int hitNdx;
    float t, epsilon, DdotN, n1, n2, localContribut, reflectContribut, refractContribut;
    float reflectCoeff, refractCoeff, filterVal, fresnelReflectance, ior, distance, reflectT, refractT;
    Ray ray = Ray(p0, d);
    epsilon = 0.001f;
    reflectT = -1.0f;
    refractT = -1.0f;

    if (depth > 0) {

        hitNdx = first_hit(ray, objList, &t);
        if (hitNdx != -1) {
            //intersection
            *tPassBack = t;
            intersectionPt = p0 + t * d;
            objNormal = objList[hitNdx]->get_normal(intersectionPt);
            
            //reflection
            reflectionVec = ray.calc_reflection(objNormal);
            reflectCoeff = (float)objList[hitNdx]->get_reflection();

            //check if entering or leaving media
            DdotN = glm::dot(d, objNormal);
            ior = (float)objList[hitNdx]->get_ior();
            if (DdotN < 0) {
                n1 = 1.0f;
                n2 = ior;
                enterMedia = true;
            }
            else {
                n1 = ior;
                n2 = 1.0f;
                objNormal = -1.0f * objNormal;
            }

            //refraction
            refractionVec = ray.calc_refraction(objNormal, n1, n2);
            refractPt = intersectionPt + refractionVec * epsilon;
            refractCoeff = (float)objList[hitNdx]->get_refraction();
            
            //calc amount of color contributions
            objColor = objList[hitNdx]->get_rgb();
            filterVal = objColor.w;
            fresnelReflectance = schlicks_aprrox(ior, objNormal, -1.0f * d);
            localContribut = (1.0f - filterVal) * (1.0f - reflectCoeff);
            reflectContribut = (1.0f - filterVal) * reflectCoeff + filterVal * fresnelReflectance;
            refractContribut = filterVal * (1.0f - fresnelReflectance);

            if (printMode) {
                std::cout << "Iteration type: " << rayType << std::endl;
                std::cout << "Ray: {" << p0.x << " " << p0.y << " " << p0.z << "} -> {";
                std::cout << d.x << " " << d.y << " " << d.z << "}\n";
                std::cout << "Hit Object: (ID #" << hitNdx + 1 << " - " << objList[hitNdx]->get_type() << ")\n";
                std::cout << "Intersection: {" << intersectionPt.x << " " << intersectionPt.y << " " << intersectionPt.z << "} at T = " << t << std::endl;
                std::cout << "Normal: {" << objNormal.x << " " << objNormal.y << " " << objNormal.z << "}\n";
            }
            
            if (altBRDF) {
                local = cook_torrance(lightList, objList[hitNdx], ray, t, objList, printMode);
            }
            else {
                local = blinn_phong(lightList, objList[hitNdx], ray, t, objList, printMode);
            }
            reflectColor = raytrace(intersectionPt + reflectionVec * epsilon, reflectionVec, &reflectT, objList, lightList, depth - 1, printMode, "Relfection", altBRDF);
            refractionColor = raytrace(refractPt, refractionVec, &refractT, objList, lightList, depth - 1, printMode, "Refraction", altBRDF);

            if (enterMedia && refractT != -1.0f) {
                distance = glm::distance(intersectionPt, refractPt + refractT * refractionVec);
                attenuation = beers_law(objColor, distance);
                color += localContribut * local + reflectContribut * reflectColor + (refractContribut * refractionColor) * attenuation;
            }
            else {
                color += localContribut * local + reflectContribut * reflectColor + refractContribut * refractionColor;
            }
        }
    }
    return color;
}