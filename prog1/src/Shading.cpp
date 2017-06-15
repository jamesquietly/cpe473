#include "Shading.h"

glm::vec3 nan_check(glm::vec3 v) {
    if (std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z)) {
        return glm::vec3(0, 0, 0);
    }
    else {
        return v;
    }
}

bool shadow_check(int hitNdx, glm::vec3 lightLoc, glm::vec3 epsPoint, glm::vec3 lightDir, float tLight) {
    bool result = false;
    // -1 means no hits along light ray, shadow check
    if (hitNdx == -1) {
        result = true;
    }
    // hit something
    else if (hitNdx >= 0) {
        float distToLight = glm::distance(lightLoc, epsPoint);
        float distToIntersect = glm::distance(epsPoint + (tLight * lightDir), epsPoint);

        //check if light ray intersection is behind the light source
        if (distToIntersect > distToLight) {
            result = true;
        }
    }

    return result;
}

glm::vec3 blinn_phong(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, glm::vec3 objNormal, std::vector<GeomObj*> objList, bool printMode) {
    glm::vec3 result, lightColor, ambColor, specColor, diffColor, lightLoc; 
    glm::vec3 lightDir, rayDir, V, H, point, sumDiff, sumSpec, normal, objColor;
    glm::vec3 epsPoint;
    glm::vec4 objColorVec4;
    glm::vec4 normalTransposed;
    glm::mat4 normalMatrix;
    float shininess, Ka, Kd, Ks, tLight, epsilon;
    int minNdx;
    Ray lightRay;
    std::vector<float> tValues;
    Intersection lightIntersect;

    Ka = (float)obj->get_ambient();
    Kd = (float)obj->get_diffuse();
    Ks = (float)obj->get_specular();
    epsilon = 0.001f;


    objColorVec4 = obj->get_rgb();
    objColor = glm::vec3(objColorVec4.x, objColorVec4.y, objColorVec4.z);
    rayDir = ray.get_direction();
    point = ray.get_pt() + (t * rayDir);


    normal = glm::normalize(objNormal);
    //return normal * 0.5f + glm::vec3(0.5);


    V = glm::normalize(-1.0f * rayDir);
    ambColor = (float)Ka * objColor;
    shininess = (2.0f / glm::pow((float)obj->get_roughness(), 2) - 2.0f);
    sumDiff = glm::vec3(0, 0, 0);
    sumSpec = glm::vec3(0, 0, 0);

    for (int i = 0; i < lightList.size(); i++) {
        lightLoc = lightList[i]->get_loc();
        lightDir = glm::normalize(lightLoc - point);
        epsPoint = point + epsilon * lightDir;
        lightRay = Ray(epsPoint, lightDir);
        lightColor = lightList[i]->get_rgb();
        H = glm::normalize(V + lightDir);

        //check to see if light ray hits any object
        lightIntersect = first_hit(lightRay, objList, &tLight);
        minNdx = lightIntersect.get_hitNdx();
        
        // -1 means no hits along light ray, shadow check
        if (shadow_check(minNdx, lightLoc, epsPoint, lightDir, tLight)) {
            diffColor = Kd * objColor  * lightColor * glm::max(0.0f, glm::dot(normal, lightDir));
            specColor = Ks * objColor  * lightColor * glm::max(0.0f, glm::pow(glm::dot(H, normal), shininess));

            sumDiff += diffColor;
            sumSpec += specColor;
        }

    }
    
    result = ambColor + sumDiff + sumSpec;
    if (printMode) {
        std::cout << "Final Color: {" << result.x << ", " << result.y << ", " << result.z << "}\n";
        std::cout << "Ambient: {" << ambColor.x << ", " << ambColor.y << ", " << ambColor.z << "}\n";
        std::cout << "Diffuse: {" << sumDiff.x << ", " << sumDiff.y << ", " << sumDiff.z << "}\n";
        std::cout << "Specular: {" << sumSpec.x << ", " << sumSpec.y << ", " << sumSpec.z << "}\n\n";
    }
    return result;
}

glm::vec3 blinn_phong_diffspec(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, glm::vec3 objNormal, std::vector<GeomObj*> objList, bool printMode) {
    glm::vec3 result, lightColor, ambColor, specColor, diffColor, lightLoc; 
    glm::vec3 lightDir, rayDir, V, H, point, sumDiff, sumSpec, normal, objColor;
    glm::vec3 epsPoint;
    glm::vec4 objColorVec4;
    glm::vec4 normalTransposed;
    glm::mat4 normalMatrix;
    float shininess, Ka, Kd, Ks, tLight, epsilon;
    int minNdx;
    Ray lightRay;
    std::vector<float> tValues;
    Intersection lightIntersect;

    Ka = (float)obj->get_ambient();
    Kd = (float)obj->get_diffuse();
    Ks = (float)obj->get_specular();
    epsilon = 0.001f;


    objColorVec4 = obj->get_rgb();
    objColor = glm::vec3(objColorVec4.x, objColorVec4.y, objColorVec4.z);
    rayDir = ray.get_direction();
    point = ray.get_pt() + (t * rayDir);


    normal = glm::normalize(objNormal);
    //return normal * 0.5f + glm::vec3(0.5);


    V = glm::normalize(-1.0f * rayDir);
    ambColor = (float)Ka * objColor;
    shininess = (2.0f / glm::pow((float)obj->get_roughness(), 2) - 2.0f);
    sumDiff = glm::vec3(0, 0, 0);
    sumSpec = glm::vec3(0, 0, 0);

    for (int i = 0; i < lightList.size(); i++) {
        lightLoc = lightList[i]->get_loc();
        lightDir = glm::normalize(lightLoc - point);
        epsPoint = point + epsilon * lightDir;
        lightRay = Ray(epsPoint, lightDir);
        lightColor = lightList[i]->get_rgb();
        H = glm::normalize(V + lightDir);

        //check to see if light ray hits any object
        lightIntersect = first_hit(lightRay, objList, &tLight);
        minNdx = lightIntersect.get_hitNdx();
        
        // -1 means no hits along light ray, shadow check
        if (shadow_check(minNdx, lightLoc, epsPoint, lightDir, tLight)) {
            diffColor = Kd * objColor  * lightColor * glm::max(0.0f, glm::dot(normal, lightDir));
            specColor = Ks * objColor  * lightColor * glm::max(0.0f, glm::pow(glm::dot(H, normal), shininess));
            sumDiff += diffColor;
            sumSpec += specColor;
        }
    }
    
    result = sumDiff + sumSpec;
    if (printMode) {
        std::cout << "Final Color: {" << result.x << ", " << result.y << ", " << result.z << "}\n";
        std::cout << "Ambient: {" << ambColor.x << ", " << ambColor.y << ", " << ambColor.z << "}\n";
        std::cout << "Diffuse: {" << sumDiff.x << ", " << sumDiff.y << ", " << sumDiff.z << "}\n";
        std::cout << "Specular: {" << sumSpec.x << ", " << sumSpec.y << ", " << sumSpec.z << "}\n\n";
    }
    return result;
}

glm::vec3 blinn_phong_soft_shadow(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, glm::vec3 objNormal, std::vector<GeomObj*> objList, bool printMode) {
    glm::vec3 result, lightColor, ambColor, specColor, diffColor; 
    glm::vec3 lightDir, rayDir, V, H, point, sumDiff, sumSpec, normal, objColor, lightLoc;
    glm::vec3 epsPoint;
    glm::vec4 objColorVec4;
    glm::vec4 normalTransposed;
    glm::mat4 normalMatrix;
    float shininess, Ka, Kd, Ks, tLight, epsilon;
    int minNdx;
    Ray lightRay;
    std::vector<float> tValues;
    Intersection lightIntersect;

    Ka = (float)obj->get_ambient();
    Kd = (float)obj->get_diffuse();
    Ks = (float)obj->get_specular();
    epsilon = 0.001f;


    objColorVec4 = obj->get_rgb();
    objColor = glm::vec3(objColorVec4.x, objColorVec4.y, objColorVec4.z);
    rayDir = ray.get_direction();
    point = ray.get_pt() + (t * rayDir);


    normal = glm::normalize(objNormal);
    //return normal * 0.5f + glm::vec3(0.5);


    V = glm::normalize(-1.0f * rayDir);
    ambColor = (float)Ka * objColor;
    shininess = (2.0f / glm::pow((float)obj->get_roughness(), 2) - 2.0f);
    sumDiff = glm::vec3(0, 0, 0);
    sumSpec = glm::vec3(0, 0, 0);
    int numSoftShadow = 10;
    for (int i = 0; i < lightList.size(); i++) {
        lightLoc = lightList[i]->get_loc();
        for (int j = lightLoc.y - (numSoftShadow/2); j < lightLoc.y + (numSoftShadow/2); j++) {
            for (int k = lightLoc.z - (numSoftShadow/2); k < lightLoc.z + (numSoftShadow/2); k++) {
                lightDir = glm::normalize(glm::vec3(lightLoc.x, j, k) - point);
                epsPoint = point + epsilon * lightDir;
                lightRay = Ray(epsPoint, lightDir);
                lightColor = lightList[i]->get_rgb();
                H = glm::normalize(V + lightDir);

                //check to see if light ray hits any object
                lightIntersect = first_hit(lightRay, objList, &tLight);
                minNdx = lightIntersect.get_hitNdx();
                
                if (shadow_check(minNdx, lightLoc, epsPoint, lightDir, tLight)) {
                    diffColor = Kd * objColor  * lightColor * glm::max(0.0f, glm::dot(normal, lightDir));
                    specColor = Ks * objColor  * lightColor * glm::max(0.0f, glm::pow(glm::dot(H, normal), shininess));

                    sumDiff += diffColor;
                    sumSpec += specColor;
                }
            }
        }

    }

    float numSoftShadowSq = numSoftShadow * numSoftShadow;
    result = ambColor + sumDiff/numSoftShadowSq + sumSpec/numSoftShadowSq;
    if (printMode) {
        std::cout << "Final Color: {" << result.x << ", " << result.y << ", " << result.z << "}\n";
        std::cout << "Ambient: {" << ambColor.x << ", " << ambColor.y << ", " << ambColor.z << "}\n";
        std::cout << "Diffuse: {" << sumDiff.x << ", " << sumDiff.y << ", " << sumDiff.z << "}\n";
        std::cout << "Specular: {" << sumSpec.x << ", " << sumSpec.y << ", " << sumSpec.z << "}\n\n";
    }
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

glm::vec3 cook_torrance(std::vector<Light*> lightList, GeomObj* obj, Ray ray, float t, std::vector<GeomObj*> objList, bool printMode) {
    glm::vec3 result, objColor, ambColor, diffColor, specColor, rayDir, point;
    glm::vec3 normal, lightColor, V, H, lightDir, sumDiffSpec, epsPoint, lightLoc;
    glm::vec4 objColorVec4;
    float s, d, roughness, ior, Rd, epsilon, tLight;
    Ray lightRay;
    std::vector<float> tValues;
    int hitNdx;
    Intersection lightIntersect;

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
        lightLoc = lightList[i]->get_loc();
        lightColor = lightList[i]->get_rgb();
        lightDir = glm::normalize(lightLoc - point);
        H = glm::normalize(V + lightDir);
        epsPoint = point + epsilon * lightDir;
        lightRay = Ray(epsPoint, lightDir);

        //check for light ray intersection with objs
        lightIntersect = first_hit(lightRay, objList, &tLight);
        hitNdx = lightIntersect.get_hitNdx();
        
        // -1 means no hit, no shadows
        if(shadow_check(hitNdx, lightLoc, epsPoint, lightDir, tLight)) {
            sumDiffSpec += ck_diff_spec(d, s, Rd, roughness, ior, normal, H, V, lightDir, lightColor, objColor);
        }   
    }

    if (printMode) {
        std::cout << "Ambient: {" << ambColor.x << ", " << ambColor.y << ", " << ambColor.z << "}\n\n";
    }

    result = ambColor + sumDiffSpec;

    return result;
}