#ifndef __GeomObj__
#define __GeomObj__

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "Ray.h"

enum tranformType {scale_t, translate_t, rotate_t};

class GeomObj{
public:
    GeomObj();
    GeomObj(std::string t);
    GeomObj(glm::vec3 c, std::vector<glm::vec4> transf, double amb, double diff, std::string typeStr);

    glm::vec3 get_rgb() const {return rgb;}
    std::vector<glm::vec4> get_transform() const {return transform;}
    double get_ambient() const {return ambient;}
    double get_diffuse() const {return diffuse;}
    std::string get_type() const {return type;}

    void set_rgb(glm::vec3 v) {rgb = glm::vec3(v.x, v.y, v.z);}
    void set_transform(std::vector<glm::vec4> t) {transform = t;}
    void set_ambient(double a) {ambient = a;}
    void set_diffuse(double d) {diffuse = d;}

    void print_transform();
    void print_color();
    void print_material();

    virtual void print() {printf("Type:%s\n", type.c_str());}
    virtual float intersect(Ray r) { return 0;}

protected:
    glm::vec3 rgb;
    std::vector<glm::vec4> transform;
    double ambient, diffuse;
    std::string type;

private:

};


class Sphere : public GeomObj {
public:
    Sphere();
    Sphere(glm::vec3 cen, glm::vec3 c, double r, double amb, double diff, std::vector<glm::vec4> transf);

    glm::vec3 get_center() const {return center;}
    double get_rad() const {return rad;}

    void set_center(glm::vec3 v) {center = v;};
    void set_rad(double r) {rad = r;};

    void print();
    float intersect(Ray r);

private:
    glm::vec3 center;
    double rad;
};


class Plane : public GeomObj {
public:
    Plane();
    Plane(glm::vec3 n, glm::vec3 c, double dis, double amb, double diff, std::vector<glm::vec4> transf);
    glm::vec3 get_normal() const {return normal;}
    double get_distance() const {return distance;}

    void set_normal(glm::vec3 v) {normal = v;}
    void set_distance(double d) {distance = d;}

    void print();
    float intersect(Ray r);


private:
    glm::vec3 normal;
    double distance;
};

#endif