#include "Plane.h"

Plane::Plane() {
    normal = glm::vec3(0, 0, 0);
    rgb = glm::vec3(0, 0, 0);
    transform = std::vector<glm::vec4>();
    distance = 0;
    ambient = 0;
    diffuse = 0;
}

Plane::Plane(glm::vec3 n, glm::vec3 c, double dis, double a, double dif, std::vector<glm::vec4> transf) {
    normal = glm::vec3(n.x, n.y, n.z);
    rgb = glm::vec3(c.x, c.y, c.z);
    transform = transf;
    distance = dis;
    ambient = a;
    diffuse = dif;
}

void Plane::set_normal(glm::vec3 v) {
    normal = v;
}

void Plane::set_rgb(glm::vec3 v) {
    rgb = v;
}

void Plane::set_transform(std::vector<glm::vec4> vectList) {
    transform = vectList;
}

void Plane::set_distance(double d) {
    distance = d;
}

void Plane::set_ambient(double d) {
    ambient = d;
}

void Plane::set_diffuse(double d) {
    diffuse = d;
}