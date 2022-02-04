#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <queue>
#include <sstream>
#include "Vector3.cpp"
#include "Ray.h"
#include "Sphere.h"

#define WIDTH 1920
#define HEIGHT 1080
#define PI 3.14159265

Color bkgcolor;

void OutputColor(std::ofstream& output_stream, const Color& color){
    int ir = static_cast<int>(255.999 * color.X());
    int ig = static_cast<int>(255.999 * color.Y());
    int ib = static_cast<int>(255.999 * color.Z());
    output_stream << ir << ' ' << ig << ' ' << ib << '\n';
}

Color trace_ray(const Ray& r, const std::vector<Object*>& objects) {
    for(int i = 0; i < objects.size(); i++){
        if(objects[i]->hit(r)){
            return objects[i]->getColor();
        }
    }
    
    return bkgcolor;
}
 
int main(int argc, char *argv[]){
    // std::vector<Object*> objects;
    // int width = 1920;
    // int height = 1080;
    // double vfov = 60;
    // Point3 eye = Point3(0,0,0);
    // Vector3 viewdir = Vector3(-1,0, -1).normalized();
    // Vector3 updir = Vector3(0,1,0);
    // Color bkgcolor = Color(0,0,1);
    // std::queue<Color> mtlcolors;

    std::vector<Object*> objects;
    int width;
    int height;
    double vfov;
    Point3 eye;
    Vector3 viewdir;
    Vector3 updir;
    std::queue<Color> mtlcolors;

    // Sphere* sphere1 = new Sphere(Point3(-2,-1, -2), 0.3);
    // objects.push_back(sphere1);
    

    



    std::ifstream inputFile;
    std::string line;
    //If file is passsed into command line argument then opens the file and gets the width and height
    if(argc > 1){
        std::string inputFileName = argv[1];
        std::ifstream inputFile;
        inputFile.open(inputFileName);
        if(inputFile.is_open()){
            std::string line;
            while(std::getline(inputFile, line)){
                if(line == ""){
                    continue;
                }
                std::stringstream ss(line);
                std::string word;
                ss >> word;
                std::cout << word << std::endl;
                if(word == "imsize"){

                    ss >> width;
                    ss >> height;
                }
                else if(word == "eye"){

                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    std::cout << x << "aa" << y << "aa" << z << '\n';
                    eye = Point3(std::stod(x),std::stod(y),std::stod(z));
                    std::cout << "eye point is:";
                    eye.print();
                }
                else if(word == "viewdir"){
                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    viewdir = Vector3(std::stod(x),std::stod(y),std::stod(z)).normalized();
                }
                else if(word == "updir"){
                    std::string x;
                    std::string y;
                    std::string z;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    updir = Vector3(std::stod(x),std::stod(y),std::stod(z)).normalized();
                }
                else if(word == "vfov"){
                    std::string deg;
                    ss >> deg;
                    vfov = std::stod(deg);
                }
                else if(word == "sphere"){
                    std::string x;
                    std::string y;
                    std::string z;
                    std::string rad;

                    ss >> x;
                    ss >> y;
                    ss >> z;
                    ss >> rad;

                    Point3 spherePos = Point3(std::stod(x),std::stod(y),std::stod(z));
                    if(mtlcolors.size() > 0){
                        Color sphereColor = mtlcolors.front();
                        mtlcolors.pop();
                        Sphere* sphere = new Sphere(spherePos, std::stod(rad), sphereColor);
                        objects.push_back(sphere);
                    }
                    else{
                        Sphere* sphere = new Sphere(spherePos, std::stod(rad));
                        objects.push_back(sphere);
                    }
                    
                    
                }
                else if(word == "bkgcolor"){

                    std::string r;
                    std::string g;
                    std::string b;

                    ss >> r;
                    ss >> g;
                    ss >> b;
                    bkgcolor = Color(std::stod(r),std::stod(g),std::stod(b));
                }
                else if(word == "mtlcolor"){

                    std::string r;
                    std::string g;
                    std::string b;

                    ss >> r;
                    ss >> g;
                    ss >> b;
                    mtlcolors.push(Color(std::stod(r),std::stod(g),std::stod(b)));
                }
                else{
                    std::cout << word << " is not a valid keyword" << std::endl;
                    return 0;
                }
            }
            
        }
        inputFile.close();
    }
    // else{
    //     height = HEIGHT;
    //     width = WIDTH;
    //     int width = 1920;
    //     int height = 1080;
    //     double vfov = 60;
    //     Point3 eye = Point3(0,0,0);
    //     Vector3 viewdir = Vector3(-1,0, -1).normalized();
    //     Vector3 updir = Vector3(0,1,0).normalized();
    //     Color bkgcolor = Color(0,0,1);
    //     Sphere* sphere1 = new Sphere(Point3(-2,-1, -2), 0.3);
    //     objects.push_back(sphere1);
    // }

    if(width <= 0 || height <= 0){
        std::cout << "width or height not valid" << std::endl;
        return 0;
    }

    //Calculate correct viewport width height and orientaion for the given vfov and viewdir/updir
    double aspectRatio = double(width) / double(height);
    double focalLength = 1.0;
    double viewportHeight = 2.0 * tan((vfov/2.0) * PI / 180.0) * focalLength; //keep
    double viewportWidth = aspectRatio * viewportHeight; //keep
    Vector3 horizontal = viewportWidth*cross(viewdir, updir);
    Vector3 vertical = viewportHeight*updir;
    Point3 lowerLeftCorner = eye - horizontal/2 - vertical/2 + viewdir*focalLength;

    std::string outputFile = "myppm.ppm";
    //Creates an output stream.
    std::ofstream output_stream(outputFile, std::ios::out | std::ios::binary);
    //Writes header to the file.
    output_stream << "P3\n" << width << std::endl << height << std::endl << 255 << std::endl;
 
    for(uint32_t y = height-1; y > 0; y--){
        //std::cerr << "\rScanlines remaining: " << y << ' ' << std::flush;
        for(uint32_t x = 0; x < width; x++){
            auto u = double(x) / (width-1);
            auto v = double(y) / (height-1);
            Ray r(eye, lowerLeftCorner + u*horizontal + v*vertical - eye);
            Color pixel = trace_ray(r, objects);
            OutputColor(output_stream, pixel);
            
        }
    }

    Vector3 test(0,0.5,-1);
    test.normalize();
    test.print();
    output_stream.close();
 
    return 0;
}
