#include "GeomObj.h"

GeomObj::GeomObj() {
    rgb = glm::vec3(0, 0, 0);
    transform = std::vector<glm::vec4>();
    ambient = 0;
    diffuse = 0;
}

GeomObj::GeomObj(glm::vec3 c, std::vector<glm::vec4> transf, double amb, double diff) {
    rgb = glm::vec3(c.x, c.y, c.z);
    ambient = amb;
    diffuse = diff;
    transform = transf;
}

void GeomObj::print_transform() {
    if (transform.size() > 0) {
        printf("- Transform:\n");
        for (int j = 0; j < transform.size(); j++) {
            if (transform[j].w == scale_t) {
                printf("  - Scale: ");
            }
            else if (transform[j].w == translate_t) {
                printf("  - Translate: ");
            }
            else if (transform[j].w == rotate_t) {
                printf("  - Rotate: ");
            }
            printf("{%.1f %.1f %.1f}\n", transform[j].x, transform[j].y, transform[j].z);
        }
    }
}

void GeomObj::print_material() {
    printf("- Material:\n");
    printf("  - Ambient: %.1f\n", ambient);
    printf("  - Diffuse: %.1f\n", diffuse);
}

void GeomObj::print_color() {
    printf("- Color: {%.1f %.1f %.1f}\n", rgb.x, rgb.y, rgb.z);
}

Sphere::Sphere() : GeomObj() {
    center = glm::vec3(0, 0, 0);
    rad = 0;

}

Sphere::Sphere(glm::vec3 cen, glm::vec3 c, double r, double amb, double diff, std::vector<glm::vec4> transf) : GeomObj(c, transf, amb, diff){
    center = glm::vec3(cen.x, cen.y, cen.z);
    rad = r;
}

void Sphere::print() {
    printf("- Type: Sphere\n");
    printf("- Center: {%.1f %.1f %.1f}\n", center.x, center.y, center.z);
    printf("- Radius %.1f\n", rad);
    GeomObj::print_color();
    GeomObj::print_material();
    GeomObj::print_transform();
}

Plane::Plane() : GeomObj() {
    normal = glm::vec3(0, 0, 0);
    distance = 0;

}

Plane::Plane(glm::vec3 n, glm::vec3 c, double dis, double amb, double diff, std::vector<glm::vec4> transf) : GeomObj(c, transf, amb, diff) {
    normal = glm::vec3(n.x, n.y, n.z);
    distance = dis;

}

void Plane::print() {
    printf("- Type: Plane\n");
    printf("- Normal: {%.1f %.1f %.1f}\n", normal.x, normal.y, normal.z);
    printf("- Distance: %.1f\n", distance);
    GeomObj::print_color();
    GeomObj::print_material();
    GeomObj::print_transform();
}