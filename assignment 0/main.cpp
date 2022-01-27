#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

#define WIDTH 1024
#define HEIGHT 1024
 
int main(int argc, char *argv[]){
    int width = 0;
    int height = 0;
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
        for(uint32_t x = 0; x < width; x++){
            //passes x and y pixel position into sin function to calculate the red and green values
            auto r = .5*sin(double(x/50.0))+.5;
            auto g = .5*sin(double(y/50.0))+.5;
            auto b = .25;
            //Multiply normalized values by 255.999 to follow ppm format.
            //Multiply by 255.999 becasue when casting to integer it cuts off the decimal values.
            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);
            //Writes the calculated color to the file.
            output_stream << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
 
    output_stream.close();
 
    return 0;
}
