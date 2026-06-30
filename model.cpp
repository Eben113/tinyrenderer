#include "model.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "draw.h"

constexpr TGAColor white   = {255, 255, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

Model::Model(std::string filename, TGAColor color, int width, int height){

    this->modelBuffer = TGAImage(width,  height, TGAImage::RGB);
    
    std::ifstream inf{filename};
    std::string line;

    std::vector<std::vector<int>> vertices;
    std::vector<int> point{0,0,0};
    int ind{};
    int points[3], val;
    //draw::line(3, 7, 5, 56, this->modelBuffer, color);
    while(inf >> line){
        if(line == "v"){
            float coord;
            for(int i = 0; i<3; i++){
                inf >> coord;
                coord = (coord+1.0)*((i==0) ?width/2 : height/2);
                point[i] = coord;
                std::cout << point[i] << "  ";
            }
            std::cout << "\n";
            vertices.push_back(point);
        }

        if(line == "f"){
            std::string discard;
            for(int i = 0; i<3; i++){
                inf >> val;
                //std::cout << val << " ";
                points[i] = val-1;
                val = 0;
                inf >> discard;
            }
            auto p1 = vertices[points[0]], p2 = vertices[points[1]], p3 = vertices[points[2]];

            draw::line(p1[0], p1[1], p2[0], p2[1], this->modelBuffer, color);
            draw::line(p2[0], p2[1], p3[0], p3[1], this->modelBuffer, color);
            draw::line(p1[0], p1[1], p3[0], p3[1], this->modelBuffer, color);

            // this->modelBuffer.set(p1[0], p1[1], white);
            // this->modelBuffer.set(p2[0], p2[1], blue);
            // this->modelBuffer.set(p3[0], p3[1], yellow);
            // break;
        }
    }
    // for(auto x: vertices){
    //     this->modelBuffer.set(x[0], x[1], white);
    // }
    inf.close();
}


void Model::write(std::string filename){
    modelBuffer.write_tga_file(filename);
    return;
}