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
#include "GeomObj.h"

enum objType {camera_t, light_t, sphere_t, plane_t, triangle_t, box_t, cone_t, comment_t};

using namespace std;

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

vector<Light*> parse_light(vector<string> lightList) {
    vector<Light*> lights;
    vector<double> vect;
    glm::vec3 loc, color;
    Light* lightObj;

    for (int i = 0; i < lightList.size(); i++) {

        vect = parse_vect(lightList[i], "light_source {");
        loc = glm::vec3(vect[0], vect[1], vect[2]);
        
        vect = parse_vect(lightList[i], "color rgb");
        color = glm::vec3(vect[0], vect[1], vect[2]);

        lightObj = new Light(loc, color);
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

vector<Plane*> parse_plane(vector<string> planeList) {
    vector<Plane*> planes;
    vector<double> vect;
    vector<glm::vec4> transform;
    glm::vec3 color, norm;
    Plane* plane;
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

        plane = new Plane(norm, color, dis, amb, diff, transform);
        planes.push_back(plane);
        transform.clear();
    }

    return planes;
}

void parse_objects(char *filename, Camera *cameraObj, 
                   vector<Light*> *lights, vector<GeomObj*> *oList) 
{
    fstream f;
    size_t findStr;
    objType t;
    string tempStr = "";
    vector<string> cameraVec, lightVec, sphereVec, planeVec, triangleVec, boxVec, coneVec;
    vector<Sphere*> sphereList;
    vector<Plane*> planeList;

    f.open(filename, fstream::in);
    if (f.is_open()) {
        for (string line; getline(f, line); ) {

            findStr = line.find("//");
            if (findStr != string::npos) {
                t = comment_t;
            }

            findStr = line.find("camera");
            if (findStr != string::npos) {
                t = camera_t;
            }

            findStr = line.find("light");
            if (findStr != string::npos) {
                t = light_t;
            }

            findStr = line.find("sphere");
            if (findStr != string::npos) {
                t = sphere_t;
            }

            findStr = line.find("plane");
            if (findStr != string::npos) {
                t = plane_t;
            }

            findStr = line.find("trianlge");
            if (findStr != string::npos) {
                t = triangle_t;
            }

            findStr = line.find("box");
            if (findStr != string::npos) {
                t = box_t;
            }

            findStr = line.find("cone");
            if (findStr != string::npos) {
                t = cone_t;
            }

            //don't include comments
            if (t != comment_t) {
                tempStr.append(line);
                tempStr.append("\n");
            }

            if (t == light_t && tempStr.compare("\n") != 0) {
                lightVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == camera_t && line.compare("}") == 0) {
                cameraVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == sphere_t && line.compare("}") == 0) {
                sphereVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == plane_t && line.compare("}") == 0) {
                planeVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == box_t && line.compare("}") == 0) {
                boxVec.push_back(tempStr);
                tempStr = "";
            }
            else if (t == cone_t && line.compare("}") == 0) {
                coneVec.push_back(tempStr);
                tempStr = "";
            }
        }

        *cameraObj = parse_camera(cameraVec);
        *lights = parse_light(lightVec);
        sphereList = parse_sphere(sphereVec);
        planeList = parse_plane(planeVec);

        for (int i = 0; i < sphereList.size(); i++) {
            oList->push_back(sphereList[i]);
        }
        for (int i = 0; i < planeList.size(); i++) {
            oList->push_back(planeList[i]);
        }
    }
    else {
        printf("Couldn't open %s\n", filename);
    }

}


void print_help() {
    printf("Usage: raytrace render <input_filename> <width> <height>\n");
    printf("       raytrace sceneinfo <input_filename>\n");
    printf("       raytrace pixelray <input_filename> <width> <height> <x> <y>\n");
    printf("       raytrace firsthit <input_filename> <width> <height> <x> <y>\n");
}

int main(int argc, char **argv) {

    int width, height;
    char *filename;
    vector<Light*> lights;
    vector<GeomObj*> objList;
    Camera cam;

    if (argc < 3) {
        print_help();
    }
    else {
        string mode(argv[1]);

        filename = argv[2];

        parse_objects(filename, &cam, &lights, &objList);

        if (mode.compare("sceneinfo") == 0) {

            cam.print();
            printf("\n");
            
            printf("%d light(s):\n", (int)lights.size());
            for (int i = 0; i < lights.size(); i++) {
                printf("Light[%d]:\n", i);
                lights[i]->print();
                printf("\n");
            }

            printf("%d object(s)\n", (int)objList.size());
            for (int i = 0; i < objList.size(); i++) {
                printf("Object[%d]:\n", i);
                objList[i]->print();
                printf("\n");
            }

        }
        else if (mode.compare("render") == 0) {
            width = atoi(argv[3]);
            height = atoi(argv[4]);

            printf("width: %d height: %d filename: %s\n", width, height, filename);

        }
    
    }
}