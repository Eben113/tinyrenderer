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

Matrix<4,4> viewport(vec<4>& point, int x, int y, int w, int h){
    Matrix<4,4> vPort = {{{w/2,0,0,x+w/2}, {0,h/2,0,y+h/2}, {0,0,1,0}, {0,0,0,0}}};
    return vPort;
}

Matrix<4,4> perspective(const double f){
    return Matrix<4,4>{{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,-1/f,1}}};
}

Matrix<4,4> lookAt(const vec<3> c, const vec<3> eye, vec<3> up){
    vec<3> n = (eye - c).normalized();
    vec<3> m = (up.cross(n)).normalized();
    vec<3> l = (n.cross(m)).normalized();
    return Matrix<4,4>{{{l.x, m.x, n.x, 0}, {l.y, m.y, n.y, 0}, {l.z, m.z, n.z, 0}, {0,0,0,1}}}* Matrix<4,4>{{{1,0,0,-c.x}, {0,1,0,-c.y}, {0,0,1,-c.z}, {0,0,0,1}}};
}

void persp(vec<3>& point, double c){
    point = point/(1 - (point.z)/c);
}
void project(vec<3>& point, int width, int height){
    for(int i = 0; i<2; ++i){point[i] = (point[i]+1)*((i==0)?(width/2):(height/2));}
    point[2] = std::min((point[2]+1)*127.5, 255.0);
    if(point[2]>=254){std::cout <<  point << "\n";}
}

Model::Model(std::string filename, int width, int height){
    std::ifstream inf{filename};
    std::string line;

    double theta = pi/6;
    Matrix<3, 3> rotate = {{{std::cos(theta), 0, std::sin(theta)}, {0,1,0}, {-std::sin(theta), 0, std::cos(theta)}}};
    std::cout << rotate << "\n";

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
            //std::cout << "raw: " << point;
            point = rotate*point;
            //std::cout << "\nrotated: " << rotate[0].dot(point);
            persp(point, 3);
            project(point, width, height);
            //std::cout << "\nprojected: " << point << "\n\n\n\n";
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

void Model::draw(TGAColor color, int width, int height, float thetaX, float thetaY, float thetaZ){
    modelBuffer = TGAImage{width, height, TGAImage::RGB};
    TGAImage grayBuffer = TGAImage(width, height, TGAImage::GRAYSCALE);
    int i = 0;
    for(auto set: points){
        std::cout << "\r";
        auto p1 = vertices[set[0]], p2 = vertices[set[1]], p3 = vertices[set[2]];
        TGAColor rnd;
        for (int c=0; c<3; c++){ rnd[c] = std::rand()%255;}
        draw::depthTriangle(p1[0], p1[1], p1[2], p2[0], p2[1], p2[2], p3[0], p3[1], p3[2], rnd, this->modelBuffer, grayBuffer);
        std::cout << i; ++i;
    }
    grayBuffer.write_tga_file("grayBuff1.tga");
}

void Model::write(std::string filename){
    this->modelBuffer.write_tga_file(filename);
    return;
}