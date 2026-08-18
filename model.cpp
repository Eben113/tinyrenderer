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

Matrix<4,4> viewport(int x, int y, int w, int h){
    Matrix<4,4> vPort = {{{w/2,0,0,x+w/2}, {0,h/2,0,y+h/2}, {0,0,1,0}, {0,0,0,1}}};
    return vPort;
}

Matrix<4,4> perspective(const double f){                                
    return Matrix<4,4>{{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,-1/f,1}}};
}

Matrix<4,4> lookAt(const vec<3> c, const vec<3> eye, vec<3> up){
    vec<3> n = (eye - c).normalized();
    vec<3> l = (up.cross(n)).normalized();
    vec<3> m = (n.cross(l)).normalized(); 
    return Matrix<4,4>{{{l.x, l.y, l.z, 0}, {m.x, m.y, m.z, 0}, {n.x, n.y, n.z, 0}, {0,0,0,1}}}* Matrix<4,4>{{{1,0,0,-c.x}, {0,1,0,-c.y}, {0,0,1,-c.z}, {0,0,0,1}}};
}


Model::Model(std::string filename, int width, int height){
    std::ifstream inf{filename};
    std::string line;

    vec<3> point;
    std::vector<int> set(3, 0);
    int val;

    while(inf >> line){
        if(line == "v"){
            float coord;
            for(int i = 0; i<3; i++){
                inf >> coord;
                point[i] = coord;
            }
            vertices.push_back(point);
        }

        if(line == "f"){
            std::string discard;
            for(int i = 0; i<3; i++){
                inf >> val;
                set[i] = val-1;
                val = 0;
                inf >> discard;
            }
            points.push_back(set);
        }
    }
    inf.close();
}

void Model::draw(Matrix<4,4>  persp, Matrix<4,4> modelView, Matrix<4,4> vPort, int width, int height, uint8_t ambient, vec<3> sun, vec<3> viewer){
    modelBuffer = TGAImage{width, height, TGAImage::GRAYSCALE};
    TGAImage grayBuffer = TGAImage(width, height, TGAImage::GRAYSCALE);
    Matrix<4,4> comp = persp * modelView;
    int progress = 0;
    TGAColor color = {ambient, ambient, ambient};
    for(auto set: points){
        vec<3> coords[3] = {vertices[set[0]], vertices[set[1]], vertices[set[2]]};
        TGAColor rnd;
        vec<4> clip[3];
        vec<3> Z{};
        for(int i = 0; i < 3; i++){
            clip[i] = comp * vec<4>{coords[i].x, coords[i].y, coords[i].z, 1.};
            clip[i] = (clip[i]/clip[i][3]);
            Z[i] = clip[i][2];
            clip[i] = vPort*clip[i];
        }
        
        vec<2> args[3];
        for(int i = 0; i< 3; ++i){
            args[i] = vec<2>{clip[i][0], clip[i][1]};
        }
        draw::rasterize(args, Z, color, modelBuffer, grayBuffer, sun, viewer);
    }
    grayBuffer.write_tga_file("grayBuff1.tga");
}

void Model::write(std::string filename){
    this->modelBuffer.write_tga_file(filename);
    return;
}