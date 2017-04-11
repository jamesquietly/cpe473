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

Sphere::Sphere() : GeomObj() {
    center = glm::vec3(0, 0, 0);
    //rgb = glm::vec3(0, 0, 0);
    //transform = std::vector<glm::vec4>();
    rad = 0;
    //ambient = 0;
    //diffuse = 0;
}

Sphere::Sphere(glm::vec3 cen, glm::vec3 c, double r, double amb, double diff, std::vector<glm::vec4> transf) : GeomObj(c, transf, amb, diff){
    center = glm::vec3(cen.x, cen.y, cen.z);
    //rgb = glm::vec3(c.x, c.y, c.z);
    rad = r;
    //ambient = amb;
    //diffuse = diff;
    //transform = transf;
}
