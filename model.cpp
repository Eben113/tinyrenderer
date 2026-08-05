#include "model.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "draw.h"
#include "linAlg.h"

constexpr TGAColor white   = {255, 255, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};
TGAImage tmp = TGAImage();
const double pi = std::acos(-1);

void project(vec<3>& point, int width, int height){
    for(int i = 3; i--; point[i] = (point[i]+1)*((i==0) ?width/2 :(i==1)? height/2: 100));
}

void rot(vec<3>& point){
    double theta = pi/6;
    Matrix<3, 3> rotate = {{{std::cos(theta), 0, -std::sin(theta)}, {0,1,0}, {-std::sin(theta), 0, std::cos(theta)}}};
    point = rotate*point;
}

Model::Model(std::string filename, int width, int height){
    std::ifstream inf{filename};
    std::string line;

    vec<3> point;
    int val;

    while(inf >> line){
        if(line == "v"){
            float coord;
            for(int i = 0; i<3; i++){
                inf >> coord;
                rot(point);
                project(point, width, height);
            }
            vertices.push_back(point);
        }

        if(line == "f"){
            std::string discard;
            for(int i = 0; i<3; i++){
                inf >> val;
                point[i] = val-1;
                val = 0;
                inf >> discard;
            }
            points.push_back(point);
        }
    }
    inf.close();
}

void Model::draw(TGAColor color, int width, int height, float thetaX, float thetaY, float thetaZ){
    modelBuffer = TGAImage{width, height, TGAImage::RGB};
    TGAImage grayBuffer = TGAImage(width, height, TGAImage::GRAYSCALE);
    for(auto set: points){
        auto p1 = vertices[set[0]], p2 = vertices[set[1]], p3 = vertices[set[2]];
        TGAColor rnd;
        for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
        draw::depthTriangle(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], p3[0], p3[1], p3[2], rnd, this->modelBuffer, grayBuffer);
    }
}

void Model::write(std::string filename){
    this->modelBuffer.write_tga_file(filename);
    return;
}