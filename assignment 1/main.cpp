#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Vector3.cpp"
#include "Ray.h"
#include "Sphere.h"

#define WIDTH 1920
#define HEIGHT 1080
#define PI 3.14159265

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
    
    Vector3 unit_direction = r.getDirection().normalized();
    auto t = 0.5*(unit_direction.Y() + 1.0);
    return (1.0-t)*Color(1.0, 1.0, 1.0) + t*Color(0.5, 0.7, 1.0);
}
 
int main(int argc, char *argv[]){
    std::vector<Object*> objects;
    int width = 1920;
    int height = 1080;
    double aspectRatio = double(width) / double(height);
    double focalLength = 1.0;
    double vfov = 60;
    Point3 eye = Point3(0,0,0);
    Vector3 viewdir = Vector3(0,0, -1).normalized();
    Vector3 updir = Vector3(0,1,0);
    Color bkgcolor = Color(0,0,1);
    Color mtlcolor = Color(1,0,0);

    Sphere* sphere1 = new Sphere(Point3(-2,-1, -2), 0.3);
    objects.push_back(sphere1);



    double viewportHeight = 2 * tan((vfov/2) * PI / 180.0) * focalLength; //keep
    double viewportWidth = aspectRatio * viewportHeight; //keep
    Vector3 horizontal = viewportWidth*cross(viewdir, updir);
    horizontal.print();
    Vector3 vertical = viewportHeight*updir;
    vertical.print();
    Point3 lowerLeftCorner = eye - horizontal/2 - vertical/2 + viewdir*focalLength;



    std::ifstream inputFile;
    std::string line;
    //If file is passsed into command line argument then opens the file and gets the width and height
    if(argc > 1){
        std::string inputFileName = argv[1];
        std::cout << inputFileName << std::endl;
        std::ifstream inputFile;
        inputFile.open(inputFileName);
        if(inputFile.is_open()){
            std::cout << "file open" << std::endl;
            std::string word;
            inputFile >> word;
            if(word == "imsize"){
                
                inputFile >> width;
                inputFile >> height;
            }
            else{
                std::cout << "Wrong Keyword" << std::endl;
                return 0;
            }
        }
        inputFile.close();
    }
    else{
        height = HEIGHT;
        width = WIDTH;
    }

    if(width <= 0 || height <= 0){
        std::cout << "width or height not valid" << std::endl;
        return 0;
    }

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
