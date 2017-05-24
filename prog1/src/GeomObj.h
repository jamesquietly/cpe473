#ifndef __GeomObj__
#define __GeomObj__

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cfloat>
#include "Ray.h"
#include "Intersection.h"

enum tranformType {scale_t, translate_t, rotate_t};

class GeomObj{
public:
    GeomObj();
    GeomObj(std::string t);
    GeomObj(glm::vec4 c, std::vector<glm::vec4> transf, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::string typeStr, glm::mat4 invMat);

    glm::vec4 get_rgb() const {return rgb;}
    std::vector<glm::vec4> get_transform() const {return transform;}
    double get_ambient() const {return ambient;}
    double get_diffuse() const {return diffuse;}
    double get_specular() const {return specular;}
    double get_roughness() const {return roughness;}
    double get_metallic() const {return metallic;}
    double get_ior() const {return ior;}
    double get_reflection() const {return reflection;}
    double get_refraction() const {return refraction;}
    std::string get_type() const {return type;}
    glm::mat4 get_inverseMatrix() const {return inverseMatrix;}

    void set_rgb(glm::vec4 v) {rgb = glm::vec4(v.x, v.y, v.z, v.w);}
    void set_transform(std::vector<glm::vec4> t) {transform = t;}
    void set_ambient(double a) {ambient = a;}
    void set_diffuse(double d) {diffuse = d;}
    void set_specular(double s) {specular = s;}
    void set_roughness(double r) {roughness = r;}
    void set_metallic(double m) {metallic = m;}
    void set_ior(double i) {ior = i;}
    void set_reflection(double r) {reflection = r;}
    void set_refraction(double r) {refraction = r;}
    void set_inverseMatrix(glm::mat4 iMat) {inverseMatrix = glm::mat4(iMat);}

    void print_transform();
    void print_color();
    void print_material();

    virtual void print() {printf("Type:%s\n", type.c_str());}
    virtual float intersect(Ray r) { return 0;}
    virtual glm::vec3 get_normal(glm::vec3 pt){return glm::vec3(0,0,0);}

protected:
    glm::vec4 rgb;
    std::vector<glm::vec4> transform;
    double ambient, diffuse, specular, roughness, metallic, ior, reflection, refraction;
    std::string type;
    glm::mat4 inverseMatrix;

private:

};


class Sphere : public GeomObj {
public:
    Sphere();
    Sphere(glm::vec3 cen, glm::vec4 c, double r, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::vector<glm::vec4> transf, glm::mat4 invMat);

    glm::vec3 get_center() const {return center;}
    double get_rad() const {return rad;}

    void set_center(glm::vec3 v) {center = v;};
    void set_rad(double r) {rad = r;};

    void print();
    float intersect(Ray r);
    glm::vec3 get_normal(glm::vec3 pt) {return glm::normalize(pt - center);}

private:
    glm::vec3 center;
    double rad;
};


class Plane : public GeomObj {
public:
    Plane();
    Plane(glm::vec3 n, glm::vec4 c, double dis, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::vector<glm::vec4> transf, glm::mat4 invMat);
    glm::vec3 get_normal() const {return normal;}
    double get_distance() const {return distance;}

    void set_normal(glm::vec3 v) {normal = v;}
    void set_distance(double d) {distance = d;}

    void print();
    float intersect(Ray r);
    glm::vec3 get_normal(glm::vec3 pt) {return normal;}


private:
    glm::vec3 normal;
    double distance;
};

class Triangle : public GeomObj {
public:
    Triangle();
    Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec4 c, double amb, double diff, double spec, double rough, double metal, double ndx, double reflect, double refrac, std::vector<glm::vec4> transf, glm::mat4 invMat);
    glm::vec3 get_pt1() const {return pt1;}
    glm::vec3 get_pt2() const {return pt2;}
    glm::vec3 get_pt3() const {return pt3;}

    void set_pt1(glm::vec3 p) {pt1.x = p.x; pt1.y = p.y; pt1.z = p.z;}
    void set_pt2(glm::vec3 p) {pt2.x = p.x; pt2.y = p.y; pt2.z = p.z;}
    void set_pt3(glm::vec3 p) {pt3.x = p.x; pt3.y = p.y; pt3.z = p.z;}

    void print();
    float intersect(Ray r);
    glm::vec3 get_normal(glm::vec3 pt);


private:
    glm::vec3 pt1, pt2, pt3;
};

float determinant(std::vector<float> vect);
int check_hit(std::vector<float> values);
Intersection first_hit(Ray ray, std::vector<GeomObj*> objList, float* newT);
glm::mat4 create_inv_mat(std::vector<glm::vec4> transformList);

#endif