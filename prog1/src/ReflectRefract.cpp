#include "ReflectRefract.h"

OptionalArgs::OptionalArgs() {
    printMode = false;
    altBRDF = false;
    useFresnel = false;
    useGI = false;
    numGISamples = 0;
    numBounces = 0;
}

OptionalArgs::OptionalArgs(bool print, bool alt, bool fresnel, bool gi, int numSamples, int nBounces) {
    printMode = print;
    altBRDF = alt;
    useFresnel = fresnel;
    useGI = gi;
    numGISamples = numSamples;
    numBounces = nBounces;
}

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

glm::vec3 cosineWeightedPoint() {
    float u, v, r, x, y, z, theta;
    u = ((float)rand()/RAND_MAX);
    v = ((float)rand()/RAND_MAX);
    r = glm::sqrt(u);
    theta = 2.0f * 3.14f * v;
    x = r * glm::cos(theta); 
    y = r * glm::sin(theta);
    z = glm::sqrt(1 - u);
    return glm::vec3(x, y, z);
}

glm::vec3 alignSampleVector(glm::vec3 sample, glm::vec3 up, glm::vec3 normal) {
    float angle = glm::acos(glm::dot(up, normal));
    glm::vec3 axis = glm::cross(up, normal);
    glm::mat4 matrix = glm::rotate(glm::mat4(1.0), angle, axis);
    glm::vec4 transformedPt = matrix * glm::vec4(sample, 0.0);
    return glm::vec3(transformedPt.x, transformedPt.y, transformedPt.z);
}

std::vector<glm::vec3> generate_hemisphere_sample_pts(int numSamples, glm::vec3 normal) {
    std::vector<glm::vec3> randomPts;
    glm::vec3 pt, up;
    up = glm::vec3(0, 0, 1);
    for (int i = 0; i < numSamples; i++) {
        pt = cosineWeightedPoint();
        pt = alignSampleVector(pt, up, normal);
        randomPts.push_back(pt);
    }
    return randomPts;
}

glm::vec3 raytrace(glm::vec3 p0, glm::vec3 d, float *tPassBack, std::vector<GeomObj*> objList, std::vector<Light*> lightList, int depth, std::string rayType, OptionalArgs optArg) {
    glm::vec3 color, local, reflectColor, reflectionVec, intersectionPt, objNormal;
    glm::vec3 refractionVec, refractionColor, attenuation, refractPt, giAmb, giSamplePt;
    glm::vec4 objColor, normalTransposed;
    glm::mat4 normalMatrix;
    bool enterMedia = false;
    color = glm::vec3(0, 0, 0);
    int hitNdx;
    float t, epsilon, DdotN, n1, n2, localContribut, reflectContribut, refractContribut;
    float reflectCoeff, refractCoeff, filterVal, fresnelReflectance = 0, ior, distance, reflectT, refractT, illumT;
    Ray ray = Ray(p0, d);
    epsilon = 0.001f;
    reflectT = -1.0f;
    refractT = -1.0f;
    illumT = 1.0f;
    Intersection intersectObj;

    Ray transformedRay;
    glm::mat4 invMat;
    glm::vec4 transformPt, transformDir;

    int giRatio = 8, currGISamples, currBounces;
    std::vector<glm::vec3> samplePts;

    if (depth > 0) {

        intersectObj = first_hit(ray, objList, &t);
        hitNdx = intersectObj.get_hitNdx();
        if (hitNdx != -1) {
            //intersection
            *tPassBack = t;
            //obj space
            //intersectionPt = p0 + t * d;


            //world space
            invMat = objList[hitNdx]->get_inverseMatrix();
            transformPt = invMat * glm::vec4(ray.get_pt(), 1.0);
            transformDir = invMat * glm::vec4(ray.get_direction(), 0.0);
            transformedRay = Ray(glm::vec3(transformPt.x, transformPt.y, transformPt.z), glm::vec3(transformDir.x, transformDir.y, transformDir.z));

            intersectionPt = transformedRay.get_pt() + t * transformedRay.get_direction(); 

            objNormal = objList[hitNdx]->get_normal(intersectionPt);

            normalMatrix = objList[hitNdx]->get_normalMatrix();
            normalTransposed = normalMatrix * glm::vec4(objNormal.x, objNormal.y, objNormal.z, 0.0f);
            objNormal = glm::vec3(normalTransposed.x, normalTransposed.y, normalTransposed.z);

            //back to obj space?
            intersectionPt = p0 + t * d;
            
            //reflection
            reflectionVec = ray.calc_reflection(objNormal);
            if (std::isnan(reflectionVec.x)) {
                reflectionVec = glm::vec3(0, 0, 0);
            }
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
            if (std::isnan(refractionVec.x)) {
                refractionVec = glm::vec3(0, 0, 0);
            }

            refractPt = intersectionPt + refractionVec * epsilon;
            refractCoeff = (float)objList[hitNdx]->get_refraction();
            
            //calc amount of color contributions
            //set fresnel = 0 for fresnel off
            objColor = objList[hitNdx]->get_rgb();
            filterVal = objColor.w;
            
            if (optArg.get_useFresnel()) {    
                fresnelReflectance = schlicks_aprrox(ior, objNormal, -1.0f * d);
            }

            localContribut = (1.0f - filterVal) * (1.0f - reflectCoeff);
            reflectContribut = (1.0f - filterVal) * reflectCoeff + filterVal * fresnelReflectance;
            refractContribut = filterVal * (1.0f - fresnelReflectance);

            if (optArg.get_printMode()) {
                std::cout << "Iteration type: " << rayType << std::endl;
                std::cout << "Ray: {" << p0.x << " " << p0.y << " " << p0.z << "} -> {";
                std::cout << d.x << " " << d.y << " " << d.z << "}\n";
                std::cout << "TransformedRay: {" << transformedRay.get_pt().x << " " << transformedRay.get_pt().y << " " << transformedRay.get_pt().z << "} -> {";
                std::cout << transformedRay.get_direction().x << " " << transformedRay.get_direction().y << " " << transformedRay.get_direction().z << "}\n";
                std::cout << "Hit Object: (ID #" << hitNdx + 1 << " - " << objList[hitNdx]->get_type() << ")\n";
                std::cout << "Intersection: {" << intersectionPt.x << " " << intersectionPt.y << " " << intersectionPt.z << "} at T = " << t << std::endl;
                std::cout << "Normal: {" << objNormal.x << " " << objNormal.y << " " << objNormal.z << "}\n";
                std::cout << "Contributions: " << localContribut << " Local, " << reflectContribut << " Reflection, " << refractContribut << " Transmission\n";
            }
            
            if (optArg.get_altBRDF()) {
                local = cook_torrance(lightList, objList[hitNdx], ray, t, objList, optArg.get_printMode());
            }
            else {
                local = blinn_phong(lightList, objList[hitNdx], ray, t, objNormal, objList, optArg.get_printMode());
            }

            reflectColor = glm::vec3(0, 0, 0);
            refractionColor = glm::vec3(0, 0, 0);
            //if obj doesn't have reflection or refract don't cast those rays
            if (reflectCoeff > 0 || filterVal > 0) {
                reflectColor = raytrace(intersectionPt + reflectionVec * epsilon, reflectionVec, &reflectT, objList, lightList, depth - 1, "Relfection", optArg);
                if (std::isnan(reflectColor.x)) {
                    reflectColor = glm::vec3(0, 0, 0);
                }
            }
            if (refractCoeff > 0 || filterVal > 0) {
                refractionColor = raytrace(refractPt, refractionVec, &refractT, objList, lightList, depth - 1, "Refraction", optArg);
                if (std::isnan(refractionColor.x)) {
                    refractionColor = glm::vec3(0, 0, 0);
                }
            }


            currGISamples = optArg.get_numGISamples();
            currBounces = optArg.get_numBounces();
            //global illumination
            if (optArg.get_useGI() && currBounces > 0) {
                optArg.set_numGISamples(currGISamples/giRatio);
                optArg.set_numBounces(currBounces - 1);

                giAmb = glm::vec3(0, 0, 0);
                local = blinn_phong_diffspec(lightList, objList[hitNdx], ray, t, objNormal, objList, optArg.get_printMode());
                samplePts = generate_hemisphere_sample_pts(currGISamples, objNormal);

                for (int i = 0; i < samplePts.size(); i++) {
                    giSamplePt = samplePts[i];
                    giAmb += raytrace(intersectionPt + (giSamplePt * epsilon), giSamplePt, &illumT, objList, lightList, currBounces - 1, "GI", optArg) * glm::dot(giSamplePt, objNormal);
                }
                giAmb = giAmb/((float)currGISamples);
                color += localContribut * (giAmb + local) + reflectContribut * reflectColor + refractContribut * refractionColor;
            }
            else {

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
    }
    return color;
}