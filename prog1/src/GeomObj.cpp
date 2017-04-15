#include "GeomObj.h"

GeomObj::GeomObj() {
    rgb = glm::vec3(0, 0, 0);
    transform = std::vector<glm::vec4>();
    ambient = 0;
    diffuse = 0;
    type = "GeomObj";
}

GeomObj::GeomObj(std::string t) {
    rgb = glm::vec3(0, 0, 0);
    transform = std::vector<glm::vec4>();
    ambient = 0;
    diffuse = 0;
    type = t;
}

GeomObj::GeomObj(glm::vec3 c, std::vector<glm::vec4> transf, double amb, double diff, std::string typeStr) {
    rgb = glm::vec3(c.x, c.y, c.z);
    ambient = amb;
    diffuse = diff;
    transform = transf;
    type = typeStr;
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
}

void GeomObj::print_color() {
    std::cout << "- Color: {";
    std::cout << rgb.x << " " << rgb.y << " " << rgb.z;
    std::cout << "}\n";
}

Sphere::Sphere() : GeomObj("Sphere") {
    center = glm::vec3(0, 0, 0);
    rad = 0;

}

Sphere::Sphere(glm::vec3 cen, glm::vec3 c, double r, double amb, double diff, std::vector<glm::vec4> transf) : GeomObj(c, transf, amb, diff, "Sphere"){
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
    //GeomObj::print_transform();
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

Plane::Plane(glm::vec3 n, glm::vec3 c, double dis, double amb, double diff, std::vector<glm::vec4> transf) : GeomObj(c, transf, amb, diff, "Plane") {
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
    //GeomObj::print_transform();
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