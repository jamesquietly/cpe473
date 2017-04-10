/* Program 1
   Author: James Ly
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "Camera.h"

enum objType {camera, light, sphere, plane, triangle, box};

using namespace std;

void parse_objects(char *filename, vector<string> *cameraVec, 
                   vector<string> *lightVec, vector<string> *sphereVec, 
                   vector<string> *planeVec, vector<string> *triangleVec, 
                   vector<string> *boxVec) 
{
    fstream f;
    size_t findStr;
    objType t;
    string tempStr = "";

    f.open(filename, fstream::in);
    for (string line; getline(f, line); ) {
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

        //don't include comments
        findStr = line.find("//");
        if (findStr == string::npos) {
            tempStr.append(line);
            tempStr.append("\n");
        }

        if (t == light) {
            lightVec->push_back(line);
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
    start = tempStr.find('<');
    tempStr.erase(start, 1);

    ss.str(tempStr);
    while (ss >> x) {
        vect.push_back(x);
        if (ss.peek() == ',') {
            ss.ignore();
        }
    }

    return vect;
}

vector<Camera> parse_camera(vector<string> cameraList) {
    vector<Camera> cameras;
    vector<double> vect;
    glm::vec3 loc, up, right, look;
    Camera cam;

    for (int i = 0; i < cameraList.size(); i++) {
        cout << cameraList[i] << endl;

        vect = parse_vect(cameraList[i], "location");
        loc = glm::vec3(vect[0], vect[1], vect[2]);
        
        vect = parse_vect(cameraList[i], "up");
        up = glm::vec3(vect[0], vect[1], vect[2]);
        
        
        vect = parse_vect(cameraList[i], "right");
        right = glm::vec3(vect[0], vect[1], vect[2]);
        

        vect = parse_vect(cameraList[i], "look_at");
        look = glm::vec3(vect[0], (float)vect[1], vect[2]);

        cam = Camera::Camera(loc, up, right, look);
        cameras.push_back(cam);

    
    }
    
    return cameras;
}

void print_help() {
    printf("Usage: raytrace render <input_filename> <width> <height>\n");
    printf("raytrace sceneinfo <input_filename>\n");
    printf("raytrace pixelray <input_filename> <width> <height> <x> <y>\n");
    printf("raytrace firsthit <input_filename> <width> <height> <x> <y>\n");
}

int main(int argc, char **argv) {
    int width, height;
    char *filename, *mode;
    vector<string> cameraVec, lightVec, sphereVec, planeVec, triangleVec, boxVec;
    vector<Camera> cameras;
    Camera cam;


    if (argc < 3) {
        print_help();
    }
    else {
        mode = argv[1];
        filename = argv[2];
        width = atoi(argv[3]);
        height = atoi(argv[4]);

        printf("width: %d height: %d filename: %s\n", width, height, filename);
        parse_objects(filename, &cameraVec, &lightVec, &sphereVec, &planeVec, &triangleVec, &boxVec);

        for (int i = 0; i < sphereVec.size(); i++) {
            cout << sphereVec[i] << endl;
        }

        cameras = parse_camera(cameraVec);
        
        for (int i = 0; i < cameras.size(); i++) {
            cam = cameras[i];
            printf("location: %f, %f, %f\n", cam.loc.x, cam.loc.y, cam.loc.z);
            printf("up: %f, %f, %f\n", cam.up.x, cam.up.y, cam.up.z);
            printf("right: %f, %f, %f\n", cam.right.x, cam.right.y, cam.right.z);
            printf("look_at: %f, %f, %f\n", cam.lookAt.x, cam.lookAt.y, cam.lookAt.z);
        } 
    }
}