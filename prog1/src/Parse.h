#ifndef __Parse__
#define __Parse__
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "Camera.h"
#include "Light.h"
#include "GeomObj.h"

enum objType {camera_t, light_t, sphere_t, plane_t, triangle_t, box_t, cone_t, comment_t};

std::vector<double> parse_vect(std::string context, std::string word);
double parse_double(std::string context, std::string word);
Camera parse_camera(std::vector<std::string> cameraList);
std::vector<Light*> parse_light(std::vector<std::string> lightList);
Sphere* parse_sphere(std::string sphereList);
Plane* parse_plane(std::string planeList);
Triangle* parse_triangle(std::string triangleList);
Box* parse_box(std::string boxList);
bool parse_objects(char *filename, Camera *cameraObj, std::vector<Light*> *lights, std::vector<GeomObj*> *oList);

#endif