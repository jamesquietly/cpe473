#include "GeomObj.h"

GeomObj::GeomObj() {
    rgb = glm::vec4(0, 0, 0, 0);
    transform = std::vector<glm::vec4>();
    ambient = 0;
    diffuse = 0;
    specular = 0;
    roughness = 0.5;
    metallic = 0.5;
    ior = 1.0;
    reflection = 0;
    refraction = 0;
    type = "GeomObj";
    inverseMatrix = glm::mat4(1.0);
    normalMatrix = glm::transpose(inverseMatrix);
}

GeomObj::GeomObj(std::string t) {
    rgb = glm::vec4(0, 0, 0, 0);
    transform = std::vector<glm::vec4>();
    ambient = 0;
    diffuse = 0;
    specular = 0;
    roughness = 0.5;
    metallic = 0.5;
    ior = 1.0;
    reflection = 0;
    refraction = 0;
    type = t;
    inverseMatrix = glm::mat4(1.0);
    normalMatrix = glm::transpose(inverseMatrix);
}

GeomObj::GeomObj(glm::vec4 c, std::vector<glm::vec4> transf, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::string typeStr, glm::mat4 invMat) {
    rgb = glm::vec4(c.x, c.y, c.z, c.w);
    ambient = amb;
    diffuse = diff;
    specular = spec;
    transform = transf;
    roughness = rough;
    metallic = metal;
    ior = ndx;
    reflection = reflect;
    refraction = refrac;
    type = typeStr;
    inverseMatrix = glm::mat4(invMat);
    normalMatrix = glm::transpose(inverseMatrix);
}

void GeomObj::print_transform() {
    if (transform.size() > 0) {
        std::cout << "- Transform:\n";
        for (int j = 0; j < transform.size(); j++) {
            if (transform[j].w == scale_t) {
                std::cout << "  - Scale: ";
            }
            else if (transform[j].w == translate_t) {
                std::cout << "  - Translate: ";
            }
            else if (transform[j].w == rotate_t) {
                std::cout << "  - Rotate: ";
            }
            std::cout << "{";
            std::cout << transform[j].x << " " << transform[j].y << " " << transform[j].z;
            std::cout << "}\n";
        }
    }
}

void GeomObj::print_material() {
    std::cout << "- Material:\n";
    std::cout << "  - Ambient: " << ambient << std::endl;
    std::cout << "  - Diffuse: " << diffuse << std::endl;
    std::cout << "  - Specular: " << specular << std::endl;
    std::cout << "  - Roughness: " << roughness << std::endl;
    std::cout << "  - Metallic: " << metallic << std::endl;
    std::cout << "  - IOR: " << ior << std::endl;
    std::cout << "  - Reflection: " << reflection << std::endl;
    std::cout << "  - Refraction: " << refraction << std::endl;
}

void GeomObj::print_color() {
    std::cout << "- Color: {";
    std::cout << rgb.x << " " << rgb.y << " " << rgb.z << " " << rgb.w;
    std::cout << "}\n";
}

Sphere::Sphere() : GeomObj("Sphere") {
    center = glm::vec3(0, 0, 0);
    rad = 0;

}

Sphere::Sphere(glm::vec3 cen, glm::vec4 c, double r, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::vector<glm::vec4> transf, glm::mat4 invMat) : GeomObj(c, transf, amb, diff, spec, rough, metal, ndx, reflect, refrac, "Sphere", invMat){
    center = glm::vec3(cen.x, cen.y, cen.z);
    rad = r;
}

void Sphere::print() {
    std::cout << "- Type: " << type << std::endl;
    std::cout << "- Center: {";
    std::cout << center.x << " " << center.y << " " << center.z << "}\n";
    std::cout << "- Radius: " << rad << std::endl;
    GeomObj::print_color();
    GeomObj::print_material();
    GeomObj::print_transform();
}

float Sphere::intersect(Ray r) {
    glm::vec3 dir, p0;
    float A, B, C, t1, t2, t, deter;

    t = -1;

    p0 = r.get_pt();
    dir = r.get_direction();

    A = glm::dot(dir, dir);
    B = glm::dot(2.0f * dir, p0 - center);
    C = glm::dot(p0 - center, p0 - center) - ((float)rad * (float)rad);

    deter = B*B - 4 * A * C;

    if (deter > 0) {
        t1 = (-1 * B + glm::sqrt(deter))/(2 * A);
        t2 = (-1 * B - glm::sqrt(deter))/(2 * A);
        
        if (t1 > 0 && t2 > 0) {
            t = std::min(t1, t2);
        }
        if ( t1 < 0 && t2 > 0) {
            t = t2;
        }
        if ( t2 < 0 && t1 > 0) {
            t = t1;
        }
        
    }

    return t;
}

Plane::Plane() : GeomObj("Plane") {
    normal = glm::vec3(0, 0, 0);
    distance = 0;

}

Plane::Plane(glm::vec3 n, glm::vec4 c, double dis, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::vector<glm::vec4> transf, glm::mat4 invMat) : GeomObj(c, transf, amb, diff, spec, rough, metal, ndx, reflect, refrac, "Plane", invMat) {
    normal = glm::vec3(n.x, n.y, n.z);
    distance = dis;

}

void Plane::print() {
    std::cout << "- Type: " << type << std::endl;
    std::cout << "- Normal: {";
    std::cout << normal.x << " " << normal.y << " " << normal.z << "}\n";
    std::cout << "- Distance: " << distance << std::endl;
    GeomObj::print_color();
    GeomObj::print_material();
    GeomObj::print_transform();
}

float Plane::intersect(Ray r) {
    float t, denom;
    glm::vec3 dir, p0;

    p0 = r.get_pt();
    dir = r.get_direction();

    denom = glm::dot(dir, normal);
    if (denom != 0) {
        t = ((float)distance - glm::dot(p0, normal))/denom;
        if (t < 0) {
            t = - 1;
        }
    }
    else {
        t = -1;
    }

    return t;

}

glm::vec3 Plane::get_min() {
    glm::vec3 min, xNorm, yNorm, zNorm;
    min = glm::vec3(0, 0, 0);
    xNorm = glm::vec3(1, 0, 0);
    yNorm = glm::vec3(0, 1, 0);
    zNorm = glm::vec3(0, 0, 1);
    if (glm::dot(normal, xNorm) == 0 && glm::dot(normal, yNorm) == 0) {
        min = glm::vec3(-10000, -10000, 0);
    }
    else if (glm::dot(normal, xNorm) == 0 && glm::dot(normal, zNorm) == 0) {
        min = glm::vec3(-10000, 0, -10000);
    }
    else if (glm::dot(normal, zNorm) == 0 && glm::dot(normal, yNorm) == 0) {
        min = glm::vec3(0, -10000, -10000);
    }
    return min;

}

glm::vec3 Plane::get_max() {
    glm::vec3 max, xNorm, yNorm, zNorm;
    max = glm::vec3(0, 0, 0);
    xNorm = glm::vec3(1, 0, 0);
    yNorm = glm::vec3(0, 1, 0);
    zNorm = glm::vec3(0, 0, 1);
    if (glm::dot(normal, xNorm) == 0 && glm::dot(normal, yNorm) == 0) {
        max = glm::vec3(10000, 10000, 0);
    }
    else if (glm::dot(normal, xNorm) == 0 && glm::dot(normal, zNorm) == 0) {
        max = glm::vec3(10000, 0, 10000);
    }
    else if (glm::dot(normal, zNorm) == 0 && glm::dot(normal, yNorm) == 0) {
        max = glm::vec3(0, 10000, 10000);
    }
    return max;
}

Triangle::Triangle() : GeomObj("Triangle") {
    pt1 = glm::vec3(0, 0, 0);
    pt2 = glm::vec3(0, 0, 0);
    pt3 = glm::vec3(0, 0, 0);
}

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec4 c, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::vector<glm::vec4> transf, glm::mat4 invMat) : GeomObj(c, transf, amb, diff, spec, rough, metal, ndx, reflect, refrac, "Triangle", invMat) {
    pt1 = glm::vec3(p1.x, p1.y, p1.z);
    pt2 = glm::vec3(p2.x, p2.y, p2.z);
    pt3 = glm::vec3(p3.x, p3.y, p3.z);
}

void Triangle::print() {
    std::cout << "- Type: " << type << std::endl;
    std::cout << "- Point 1: <" << pt1.x << " " << pt1.y << " " << pt1.z << ">\n";
    std::cout << "- Point 2: <" << pt2.x << " " << pt2.y << " " << pt2.z << ">\n";
    std::cout << "- Point 3: <" << pt3.x << " " << pt3.y << " " << pt3.z << ">\n";
    GeomObj::print_color();
    GeomObj::print_material();
    GeomObj::print_transform();
}

float Triangle::intersect(Ray r) {
    float t, detA, gamma, beta;
    glm::vec3 vertA, vertB, vertC, p0, rayDir;
    std::vector<float> A, tNumerator, betaNumerator, gamNumerator;

    vertA = pt1;
    vertB = pt2;
    vertC = pt3;
    p0 = r.get_pt();
    rayDir = r.get_direction();

    A = {vertA.x - vertB.x, vertA.x - vertC.x, rayDir.x, 
         vertA.y - vertB.y, vertA.y - vertC.y, rayDir.y,
         vertA.z - vertB.z, vertA.z - vertC.z, rayDir.z};

    tNumerator = {vertA.x - vertB.x, vertA.x - vertC.x, vertA.x - p0.x,
                  vertA.y - vertB.y, vertA.y - vertC.y, vertA.y - p0.y,
                  vertA.z - vertB.z, vertA.z - vertC.z, vertA.z - p0.z};

    gamNumerator = {vertA.x - vertB.x, vertA.x - p0.x, rayDir.x,
                    vertA.y - vertB.y, vertA.y - p0.y, rayDir.y,
                    vertA.z - vertB.z, vertA.z - p0.z, rayDir.z};

    betaNumerator = {vertA.x - p0.x, vertA.x - vertC.x, rayDir.x,
                     vertA.y - p0.y, vertA.y - vertC.y, rayDir.y,
                     vertA.z - p0.z, vertA.z - vertC.z, rayDir.z};

    detA = determinant(A);

    t = determinant(tNumerator)/detA;
    if (t < 0) {
        return -1;
    }
    
    gamma = determinant(gamNumerator)/detA;
    if (gamma < 0 || gamma > 1) {
        return -1;
    }

    beta = determinant(betaNumerator)/detA;
    if (beta < 0 || beta > 1 - gamma) {
        return -1;
    }


    return t;
}

glm::vec3 Triangle::get_normal(glm::vec3 pt) {
    glm::vec3 AB, AC, normal;
    AB = pt2 - pt1;
    AC = pt3 - pt1;

    normal = glm::normalize(glm::cross(AB, AC));

    return normal;
}

glm::vec3 Triangle::get_min() {
    glm::vec3 min = glm::vec3(0, 0, 0);
    min.x = std::min(pt1.x, std::min(pt2.x, pt3.x));
    min.y = std::min(pt1.y, std::min(pt2.y, pt3.y));
    min.z = std::min(pt1.z, std::min(pt2.z, pt3.z));
    return min;
}

glm::vec3 Triangle::get_max() {
    glm::vec3 max = glm::vec3(0, 0, 0);
    max.x = std::max(pt1.x, std::max(pt2.x, pt3.x));
    max.y = std::max(pt1.y, std::max(pt2.y, pt3.y));
    max.z = std::max(pt1.z, std::max(pt2.z, pt3.z));
    return max;
}


Box::Box() : GeomObj("Box") {
    min = glm::vec3(0, 0, 0);
    max = glm::vec3(0, 0, 0);
}

Box::Box(glm::vec3 mins, glm::vec3 maxes, glm::vec4 c, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::vector<glm::vec4> transf, glm::mat4 invMat) : GeomObj(c, transf, amb, diff, spec, rough, metal, ndx, reflect, refrac, "Box", invMat) {
    min = glm::vec3(mins);
    max = glm::vec3(maxes);
}

void Box::print() {
    std::cout << "- Type: " << type << std::endl;
    std::cout << "- Min: {" << min.x << ", " << min.y << ", " << min.z << "}\n";
    std::cout << "- Max: {" << max.x << ", " << max.y << ", " << max.z << "}\n";
    GeomObj::print_color();
    GeomObj::print_material();
    GeomObj::print_transform();
}

float Box::intersect(Ray r) {
    glm::vec3 rayPt, rayDir;
    rayPt = r.get_pt();
    rayDir = r.get_direction();
    float returnVal = -1;

    if (rayDir.x == 0) {
        if (rayPt.x >= min.x || rayPt.x <= max.x) {
            return -1;
        }
    }
    if (rayDir.y == 0) {
        if (rayPt.y >= min.y || rayPt.y <= max.y) {
            return -1;
        }
    }
    if (rayDir.z == 0) {
        if (rayPt.z >= min.z || rayPt.z <= max.z) {
            return -1;
        }
    }
    else {
        float tgmin = -std::numeric_limits<float>::infinity();
        float tgmax = std::numeric_limits<float>::infinity();

        //for x
        float t1 = (min.x - rayPt.x)/rayDir.x;
        float t2 = (max.x - rayPt.x)/rayDir.x;

        //want the closest t1 from the ray's perspective
        if (t1 > t2) {
            std::swap(t1, t2);
        }

        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }

        //for y
        t1 = (min.y - rayPt.y)/rayDir.y;
        t2 = (max.y - rayPt.y)/rayDir.y;
        
        //want the closest t1 from the ray's perspective
        if (t1 > t2) {
            std::swap(t1, t2);
        }

        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }

        //for z
        t1 = (min.z - rayPt.z)/rayDir.z;
        t2 = (max.z - rayPt.z)/rayDir.z;
        
        //want the closest t1 from the ray's perspective
        if (t1 > t2) {
            std::swap(t1, t2);
        }

        if (t1 > tgmin) {
            tgmin = t1;
        }
        if (t2 < tgmax) {
            tgmax = t2;
        }


        if (tgmin > tgmax) {
            return -1;
        }
        //box is behind
        if (tgmax < 0) {
            return -1;
        }

        //if we got here, there's an intersection
        if (tgmin > 0) {
            returnVal = tgmin;
        }
        else {
            returnVal = tgmax; //could be inside, so tgmax is the intersection we will hit
        }
    }
    return returnVal;
}

glm::vec3 Box::get_normal(glm::vec3 pt) {
    glm::vec3 normal = glm::vec3(0, 0, 0);
    float epsilon = 0.001f;

    if (epsilonEquals(pt.x, min.x, epsilon)) {
        normal = glm::vec3(-1, 0, 0);
    }
    else if (epsilonEquals(pt.x, max.x, epsilon)) {
        normal = glm::vec3(1, 0, 0);
    }
    else if (epsilonEquals(pt.y, min.y, epsilon)) {
        normal = glm::vec3(0, -1, 0);
    }
    else if (epsilonEquals(pt.y, max.y, epsilon)) {
        normal = glm::vec3(0, 1, 0);
    }
    else if (epsilonEquals(pt.z, min.z, epsilon)) {
        normal = glm::vec3(0, 0, -1);
    }
    else if (epsilonEquals(pt.z, max.z, epsilon)) {
        normal = glm::vec3(0, 0, 1);
    }
    return normal;
}


float determinant(std::vector<float> vect) {
    float a, b, c, d, e, f, g, h, i;

    if (vect.size() < 9) {
        std::cout << "not 3x3 matrix" << std::endl;
        return -1;
    }

    a = vect[0];
    b = vect[1];
    c = vect[2];
    d = vect[3];
    e = vect[4];
    f = vect[5];
    g = vect[6];
    h = vect[7];
    i = vect[8];

    return a*e*i - a*f*h + b*f*g - b*d*i + c*d*h - c*e*g;
}

/* check list of t values to see if there are any hits */
int check_hit(std::vector<float> values) {
    float noHit = -1;
    int minNdx = -1;
    float minValue = FLT_MAX;

    for (int i = 0; i < values.size(); i++) {
        if (values[i] != noHit && values[i] < minValue) {
            minValue = values[i];
            minNdx = i;
        }
    }

    return minNdx;
}

Intersection first_hit(Ray ray, std::vector<GeomObj*> objList, float* newT) {
    float t;
    int hitNdx;
    std::vector<float> tValues;
    std::vector<Ray> transformedRayList;
    Ray transformedRay;
    glm::mat4 invMat;
    glm::vec4 transformPt, transformDir;
    Intersection intersectObj;

    for (int i = 0; i < objList.size(); i++) {
        invMat = objList[i]->get_inverseMatrix();
        transformPt = invMat * glm::vec4(ray.get_pt(), 1.0);
        transformDir = invMat * glm::vec4(ray.get_direction(), 0.0);
        transformedRay = Ray(glm::vec3(transformPt.x, transformPt.y, transformPt.z), glm::normalize(glm::vec3(transformDir.x, transformDir.y, transformDir.z)) ); 
        t = objList[i]->intersect(transformedRay);
        //t = objList[i]->intersect(ray);
        tValues.push_back(t);
        transformedRayList.push_back(transformedRay);
    }

    hitNdx = check_hit(tValues);
    intersectObj = Intersection();
    if (hitNdx != -1) {
        *newT = tValues[hitNdx];
        transformedRay = transformedRayList[hitNdx];
        intersectObj = Intersection(ray, transformedRay, tValues[hitNdx], hitNdx);
        //std::cout << "transformedRay: " << transformedRay.get_pt().x <<  " " << transformedRay.get_pt().y << " " << transformedRay.get_pt().z << std::endl; 
    }

    return intersectObj;
}

glm::mat4 create_inv_mat(std::vector<glm::vec4> transformList) {
    glm::mat4 transformMat = glm::mat4(1.0);
    glm::mat4 scaleMat, rotateMat, translateMat;
    glm::vec4 vect;
    glm::vec3 axis;
    float angle;

    for (int i = 0; i < transformList.size(); i++) {
        vect = transformList[i];
        rotateMat = glm::mat4(1.0);
        if (vect.w == scale_t) {
            scaleMat = glm::scale(glm::mat4(1.0), glm::vec3(vect.x, vect.y, vect.z));
            transformMat = scaleMat * transformMat;
        }
        else if (vect.w == rotate_t) {

            angle = glm::radians(vect.x);
            axis = glm::vec3(1, 0, 0);
            rotateMat = glm::rotate(glm::mat4(1.0), angle, axis) * rotateMat;

            angle = glm::radians(vect.y);
            axis = glm::vec3(0, 1, 0);
            rotateMat = glm::rotate(glm::mat4(1.0), angle, axis) * rotateMat;

            angle = glm::radians(vect.z);
            axis = glm::vec3(0, 0, 1);
            rotateMat = glm::rotate(glm::mat4(1.0), angle, axis) * rotateMat;

            transformMat = rotateMat * transformMat;
        }
        else if (vect.w == translate_t) {
            translateMat = glm::translate(glm::mat4(1.0), glm::vec3(vect.x, vect.y, vect.z));
            transformMat = translateMat * transformMat;
        }
    }

    return glm::inverse(transformMat);
}

bool epsilonEquals(float a, float b, float epsilon) {
    return std::abs(a - b) < epsilon;
}