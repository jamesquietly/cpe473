/* Program 1
   Author: James Ly
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Camera.h"
#include "Light.h"
#include "Plane.h"
#include "GeomObj.h"

enum objType {camera, light, sphere, plane, triangle, box, cone, comment};
enum tranformType {scale_t, translate_t, rotate_t};

using namespace std;


void parse_objects(char *filename, vector<string> *cameraVec, 
                   vector<string> *lightVec, vector<string> *sphereVec, 
                   vector<string> *planeVec, vector<string> *triangleVec, 
                   vector<string> *boxVec, vector<string> *coneVec) 
{
    fstream f;
    size_t findStr;
    objType t;
    string tempStr = "";

    f.open(filename, fstream::in);
    for (string line; getline(f, line); ) {

        findStr = line.find("//");
        if (findStr != string::npos) {
            t = comment;
        }

        findStr = line.find("camera");
        if (findStr != string::npos) {
            t = camera;
        }

        findStr = line.find("light");
        if (findStr != string::npos) {
            t = light;
        }

        findStr = line.find("sphere");
        if (findStr != string::npos) {
            t = sphere;
        }

        findStr = line.find("plane");
        if (findStr != string::npos) {
            t = plane;
        }

        findStr = line.find("trianlge");
        if (findStr != string::npos) {
            t = triangle;
        }

        findStr = line.find("box");
        if (findStr != string::npos) {
            t = box;
        }

        findStr = line.find("cone");
        if (findStr != string::npos) {
            t = cone;
        }

        //don't include comments
        if (t != comment) {
            tempStr.append(line);
            tempStr.append("\n");
        }

        if (t == light && tempStr.compare("\n") != 0) {
            lightVec->push_back(tempStr);
            tempStr = "";
        }
        else if (t == camera && line.compare("}") == 0) {
            cameraVec->push_back(tempStr);
            tempStr = "";
        }
        else if (t == sphere && line.compare("}") == 0) {
            sphereVec->push_back(tempStr);
            tempStr = "";
        }
        else if (t == plane && line.compare("}") == 0) {
            planeVec->push_back(tempStr);
            tempStr = "";
        }
        else if (t == box && line.compare("}") == 0) {
            boxVec->push_back(tempStr);
            tempStr = "";
        }
        else if (t == cone && line.compare("}") == 0) {
            coneVec->push_back(tempStr);
            tempStr = "";
        }
    }
}

vector<double> parse_vect(string context, string word) {
    vector<double> vect;
    int start;
    double x;
    string tempStr;
    stringstream ss;

    start = context.find(word);
    tempStr = context.substr(start, string::npos);
    start = tempStr.find('<');
    tempStr = tempStr.substr(start, string::npos);
    //skip over '<' in the string
    start = tempStr.find('<');
    tempStr = tempStr.substr(start + 1, string::npos);

    ss.str(tempStr);
    while (ss >> x) {
        vect.push_back(x);
        if (ss.peek() == ',') {
            ss.ignore();
        }
    }

    return vect;
}

double parse_double(string context, string word) {
    double result;
    int start;
    string tempStr;
    stringstream ss;

    start = context.find(word);
    tempStr = context.substr(start, string::npos);
    start = tempStr.find(" ");
    tempStr = tempStr.substr(start, string::npos);

    ss.str(tempStr);
    ss >> result;

    return result;
}

Camera parse_camera(vector<string> cameraList) {
    vector<double> vect;
    glm::vec3 loc, up, right, look;
    Camera cam;


    vect = parse_vect(cameraList[0], "location");
    loc = glm::vec3(vect[0], vect[1], vect[2]);
    
    vect = parse_vect(cameraList[0], "up");
    up = glm::vec3(vect[0], vect[1], vect[2]);
    
    
    vect = parse_vect(cameraList[0], "right");
    right = glm::vec3(vect[0], vect[1], vect[2]);
    

    vect = parse_vect(cameraList[0], "look_at");
    look = glm::vec3(vect[0], (float)vect[1], vect[2]);

    cam = Camera::Camera(loc, up, right, look);

    
    
    return cam;
}

vector<Light> parse_light(vector<string> lightList) {
    vector<Light> lights;
    vector<double> vect;
    glm::vec3 loc, color;
    Light lightObj;

    for (int i = 0; i < lightList.size(); i++) {

        vect = parse_vect(lightList[i], "light_source {");
        loc = glm::vec3(vect[0], vect[1], vect[2]);
        
        vect = parse_vect(lightList[i], "color rgb");
        color = glm::vec3(vect[0], vect[1], vect[2]);

        lightObj = Light(loc, color);
        lights.push_back(lightObj);
    }

    return lights;
}

vector<Sphere*> parse_sphere(vector<string> sphereList) {
    vector<Sphere*> spheres;
    vector<double> vect;
    vector<glm::vec4> transform;
    glm::vec3 color, cen;
    Sphere* sphere;
    int pos = 0;
    string delimiter = "\n", tempStr;
    double rad, amb, diff;


    for (int i = 0; i < sphereList.size(); i++) {

        vect = parse_vect(sphereList[i], "color rgb");
        color = glm::vec3(vect[0], vect[1], vect[2]);

        vect = parse_vect(sphereList[i], "sphere {");
        cen = glm::vec3(vect[0], vect[1], vect[2]);

        rad = parse_double(sphereList[i], ">,");

        amb = parse_double(sphereList[i], "ambient");

        diff = parse_double(sphereList[i], "diffuse");

        while ((pos = sphereList[i].find(delimiter)) != string::npos ) {
            tempStr = sphereList[i].substr(0, pos);
            if (tempStr.find("scale") != string::npos) {
                vect = parse_vect(tempStr, "scale");
                transform.push_back(glm::vec4(vect[0], vect[1], vect[2], scale_t));
            }
            else if (tempStr.find("translate") != string::npos) {
                vect = parse_vect(tempStr, "translate");
                transform.push_back(glm::vec4(vect[0], vect[1], vect[2], translate_t));
            }
            else if (tempStr.find("rotate") != string::npos) {
                vect = parse_vect(tempStr, "rotate");
                transform.push_back(glm::vec4(vect[0], vect[1], vect[2], rotate_t));
            }
            sphereList[i].erase(0, pos + delimiter.length());
        }


        sphere = new Sphere(cen, color, rad, amb, diff, transform);
        spheres.push_back(sphere);

        transform.clear();
    }
    return spheres;
}

vector<Plane> parse_plane(vector<string> planeList) {
    vector<Plane> planes;
    vector<double> vect;
    vector<glm::vec4> transform;
    glm::vec3 color, norm;
    Plane plane;
    double dis, amb, diff;
    int pos = 0;
    string tempStr, delimiter = "\n";
    
    for (int i = 0; i < planeList.size(); i++) {
        vect = parse_vect(planeList[i], "plane {");
        norm = glm::vec3(vect[0], vect[1], vect[2]);
        
        vect = parse_vect(planeList[i], "color rgb");
        color = glm::vec3(vect[0], vect[1], vect[2]);

        dis = parse_double(planeList[i], ">,");

        amb = parse_double(planeList[i], "ambient");

        diff = parse_double(planeList[i], "diffuse");

        while ((pos = planeList[i].find(delimiter)) != string::npos ) {
            tempStr = planeList[i].substr(0, pos);
            if (tempStr.find("scale") != string::npos) {
                vect = parse_vect(tempStr, "scale");
                transform.push_back(glm::vec4(vect[0], vect[1], vect[2], scale_t));
            }
            else if (tempStr.find("translate") != string::npos) {
                vect = parse_vect(tempStr, "translate");
                transform.push_back(glm::vec4(vect[0], vect[1], vect[2], translate_t));
            }
            else if (tempStr.find("rotate") != string::npos) {
                vect = parse_vect(tempStr, "rotate");
                transform.push_back(glm::vec4(vect[0], vect[1], vect[2], rotate_t));
            }
            planeList[i].erase(0, pos + delimiter.length());
        }

        plane = Plane(norm, color, dis, amb, diff, transform);
        planes.push_back(plane);
        transform.clear();
    }

    return planes;
}

void print_help() {
    printf("Usage: raytrace render <input_filename> <width> <height>\n");
    printf("raytrace sceneinfo <input_filename>\n");
    printf("raytrace pixelray <input_filename> <width> <height> <x> <y>\n");
    printf("raytrace firsthit <input_filename> <width> <height> <x> <y>\n");
}

int main(int argc, char **argv) {
    int width, height, numObj, objNdx = 0, lightNdx = 0;
    char *filename;
    vector<string> cameraVec, lightVec, sphereVec, planeVec, triangleVec, boxVec, coneVec;
    vector<Light> lights;
    vector<Sphere*> spheres;
    vector<Plane> planes;
    vector<glm::vec4> transform;
    Camera cam;
    Light light;
    Sphere sphere;
    Plane plane;


    if (argc < 3) {
        print_help();
    }
    else {
        string mode(argv[1]);

        filename = argv[2];

        parse_objects(filename, &cameraVec, &lightVec, &sphereVec, &planeVec, &triangleVec, &boxVec, &coneVec);

        cam = parse_camera(cameraVec);
        lights = parse_light(lightVec);
        spheres = parse_sphere(sphereVec);
        planes = parse_plane(planeVec);

        numObj = spheres.size() + planes.size();
        if (mode.compare("sceneinfo") == 0) {
            printf("Camera:\n");
            printf("- Location: {%.1f %.1f %.1f}\n", cam.get_loc().x, cam.get_loc().y, cam.get_loc().z);
            printf("- Up: {%.1f %.1f %.1f}\n", cam.get_up().x, cam.get_up().y, cam.get_up().z);
            printf("- Right: {%.5f %.1f %.1f}\n", cam.get_right().x, cam.get_right().y, cam.get_right().z);
            printf("- Look at: {%.1f %.1f %.1f}\n", cam.get_lookAt().x, cam.get_lookAt().y, cam.get_lookAt().z);
            printf("\n");


            printf("%d light(s):\n", (int)lights.size());
            for (int i = 0; i < lights.size(); i++) {
                light = lights[i];
                printf("Light[%d]:\n", lightNdx++);
                printf("- Location: {%.1f %.1f %.1f}\n", light.get_loc().x, light.get_loc().y, light.get_loc().z);
                printf("- Color: {%.1f %.1f %.1f}\n", light.get_rgb().x, light.get_rgb().y, light.get_rgb().z);
                printf("\n");
            }


            printf("%d object(s):\n", numObj);
            for (int i = 0; i < spheres.size(); i++) {
                sphere = *spheres[i];
                //transform = sphere.get_transform();

                printf("Object[%d]:\n", objNdx++);
                printf("- Type: Sphere\n");
                printf("- Center: {%.1f %.1f %.1f}\n", sphere.get_center().x, sphere.get_center().y, sphere.get_center().z);
                printf("- Radius %.1f\n", sphere.get_rad());
                printf("- Color: {%.1f %.1f %.1f}\n", sphere.get_rgb().x, sphere.get_rgb().y, sphere.get_rgb().z);
                printf("- Material:\n");
                printf("  - Ambient: %.1f\n", sphere.get_ambient());
                printf("  - Diffuse: %.1f\n", sphere.get_diffuse());
                printf("- Transform:\n");
                /*
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
                }*/

                printf("\n");
            }

            for (int i = 0; i < planes.size(); i++) {
                plane = planes[i];
                transform = planes[i].get_transform();

                printf("Object[%d]:\n", objNdx++);
                printf("- Type: Plane\n");
                printf("- Normal: {%.1f %.1f %.1f}\n", plane.get_normal().x, plane.get_normal().y, plane.get_normal().z);
                printf("- Distance: %.1f\n", plane.get_distance());
                printf("- Color: {%.1f %.1f %.1f}\n", plane.get_rgb().x, plane.get_rgb().y, plane.get_rgb().z);
                printf("- Material:\n");
                printf("  - Ambient: %.1f\n", plane.get_ambient());
                printf("  - Diffuse: %.1f\n", plane.get_diffuse());
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

            printf("\n");
        }
        else {
            width = atoi(argv[3]);
            height = atoi(argv[4]);

            printf("width: %d height: %d filename: %s\n", width, height, filename);
        }

    }
}