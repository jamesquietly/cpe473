#include "Shading.h"

glm::vec3 blinn_phong(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, std::vector<GeomObj*> objList) {
    glm::vec3 result, lightColor, ambColor, specColor, diffColor; 
    glm::vec3 lightDir, rayDir, V, H, point, sumDiff, sumSpec, normal, objColor;
    glm::vec3 epsPoint;
    glm::vec4 objColorVec4;
    float shininess, Ka, Kd, Ks, tLight, epsilon, distToLight, distToIntersect;
    int minNdx;
    Ray lightRay;
    std::vector<float> tValues;

    Ka = (float)obj->get_ambient();
    Kd = (float)obj->get_diffuse();
    Ks = (float)obj->get_specular();
    epsilon = 0.001f;

    objColorVec4 = obj->get_rgb();
    objColor = glm::vec3(objColorVec4.x, objColorVec4.y, objColorVec4.z);
    rayDir = ray.get_direction();
    point = ray.get_pt() + (t * rayDir);
    normal = obj->get_normal(point);
    V = glm::normalize(-1.0f * rayDir);
    ambColor = (float)Ka * objColor;
    shininess = (2.0f / glm::pow((float)obj->get_roughness(), 2) - 2.0f);
    sumDiff = glm::vec3(0, 0, 0);
    sumSpec = glm::vec3(0, 0, 0);

    for (int i = 0; i < lightList.size(); i++) {
        lightDir = lightList[i]->get_loc() - point;
        lightDir = glm::normalize(lightDir);
        epsPoint = point + epsilon * lightDir;
        lightRay = Ray(epsPoint, lightDir);
        lightColor = lightList[i]->get_rgb();
        H = glm::normalize(V + lightDir);

        //check to see if light ray hits any object
        for (int j = 0; j < objList.size(); j++) {
            tLight = objList[j]->intersect(lightRay);
            tValues.push_back(tLight);
        }
        minNdx = check_hit(tValues);

        
        // -1 means no hits along light ray, shadow check
        if (minNdx == -1) {
            diffColor = Kd * objColor  * lightColor * glm::max(0.0f, glm::dot(normal, lightDir));
            specColor = Ks * objColor  * lightColor * glm::max(0.0f, glm::pow(glm::dot(H, normal), shininess));
            sumDiff += diffColor;
            sumSpec += specColor;
        }
        else if (minNdx >= 0) {
            distToLight = glm::distance(lightList[i]->get_loc(), epsPoint);
            distToIntersect = glm::distance(epsPoint + (tValues[minNdx] * lightDir), epsPoint);

            //check if light ray intersection is behind the light source
            if (distToIntersect > distToLight) {
                diffColor = Kd * objColor  * lightColor * glm::max(0.0f, glm::dot(normal, lightDir));
                specColor = Ks * objColor  * lightColor * glm::max(0.0f, glm::pow(glm::dot(H, normal), shininess));
                sumDiff += diffColor;
                sumSpec += specColor;
            } 

        } 

    }
    
    result = ambColor + sumDiff + sumSpec;
    return result;
}


//helper func to calculate diffuse and specular in cook torrance
glm::vec3 ck_diff_spec(float d, float s, float Rd, float roughness, float ior, glm::vec3 normal, glm::vec3 H, glm::vec3 V, glm::vec3 lightDir, glm::vec3 lightColor, glm::vec3 objColor) {

    glm::vec3 diffSpecColor;
    float DBeck, G, F, G1, G2, F0, NdotH, tan, Rs;

    NdotH = glm::clamp(glm::dot(normal, H), 0.0f, 1.0f);
    if (NdotH == 0) {
        DBeck = 0.0f;
    }
    else {
        tan = (glm::pow(NdotH, 2.0f) - 1.0f) / glm::pow(NdotH, 2.0f);
        DBeck = (1.0f/(3.14159f * glm::pow(roughness, 2.0f))) * glm::pow(2.71828f, tan / glm::pow(roughness, 2.0f)) / glm::pow(NdotH, 4.0f) ;
    }
    
    G1 = (2.0f * NdotH * glm::clamp(glm::dot(normal, V), 0.0f, 1.0f)) / glm::clamp(glm::dot(V, H), 0.0f, 1.0f);
    G2 = (2.0f * NdotH * glm::clamp(glm::dot(normal, lightDir), 0.0f, 1.0f)) / glm::clamp(glm::dot(V, H), 0.0f, 1.0f) ;
    G = glm::min(1.0f, glm::min(G1, G2));

    F0 = glm::pow(ior - 1.0f, 2.0f)/glm::pow(ior + 1.0f, 2.0f);
    F = F0 + (1.0f - F0) * glm::pow(1.0f - glm::clamp(glm::dot(V, H), 0.1f, 1.0f), 5.0f);

    Rs = (DBeck * G * F) / (4.0f * glm::clamp(glm::dot(normal, V), 0.0f, 1.0f));
    diffSpecColor = lightColor * objColor * ((d * glm::clamp(glm::dot(normal, lightDir), 0.0f, 1.0f) * Rd) + (s * Rs));

    return diffSpecColor;
}

glm::vec3 cook_torrance(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, std::vector<GeomObj*> objList) {
    glm::vec3 result, objColor, ambColor, diffColor, specColor, rayDir, point;
    glm::vec3 normal, lightColor, V, H, lightDir, sumDiffSpec, epsPoint;
    glm::vec4 objColorVec4;
    float s, d, roughness, ior, Rd, epsilon, tLight, distToLight, distToIntersect;
    Ray lightRay;
    std::vector<float> tValues;
    int hitNdx;

    s = (float)obj->get_metallic();
    d = 1.0f - s;
    objColorVec4 = obj->get_rgb();
    objColor = glm::vec3(objColorVec4.x, objColorVec4.y, objColorVec4.z);
    ambColor = (float)obj->get_ambient() * objColor;
    Rd = (float)obj->get_diffuse();
    rayDir = ray.get_direction();
    point = ray.get_pt() + (t * rayDir);
    normal = obj->get_normal(point);
    V = glm::normalize(-1.0f * rayDir);
    roughness = glm::pow((float)obj->get_roughness(), 2.0f);
    ior = (float)obj->get_ior();
    epsilon = 0.001f;
    sumDiffSpec = glm::vec3(0, 0, 0);

    for (int i = 0; i < lightList.size(); i++) {
        lightColor = lightList[i]->get_rgb();
        lightDir = lightList[i]->get_loc() - point;
        lightDir = glm::normalize(lightDir);
        H = glm::normalize(V + lightDir);
        epsPoint = point + epsilon * lightDir;
        lightRay = Ray(epsPoint, lightDir);

        //check for light ray intersection with objs
        for (int j = 0; j < objList.size(); j++) {
            tLight = objList[j]->intersect(lightRay);
            tValues.push_back(tLight);
        }
        hitNdx = check_hit(tValues);
        
        // -1 means no hit, no shadows
        if(hitNdx == -1) {
            sumDiffSpec += ck_diff_spec(d, s, Rd, roughness, ior, normal, H, V, lightDir, lightColor, objColor);
        }
        else if (hitNdx) {
            distToLight = glm::distance(lightList[i]->get_loc(), epsPoint);
            distToIntersect = glm::distance(epsPoint + (tValues[hitNdx] * lightDir), epsPoint);

            //check to see if obj is behing light source
            if (distToIntersect > distToLight) {
                sumDiffSpec += ck_diff_spec(d, s, Rd, roughness, ior, normal, H, V, lightDir, lightColor, objColor);
            }
        }
        
    }

    result = ambColor + sumDiffSpec;

    return result;
}