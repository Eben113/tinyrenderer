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
    std::vector<int> set1(3, 0);
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

        else if(line == "vn"){
            float coord;
            for(int i = 0; i<3; i++){
                inf >> coord;
                point[i] = coord;
            }
            normals.push_back(point);
        }

        else if(line == "f"){
            std::string discard;
            for(int i = 0; i<3; i++){
                inf >> val;
                set[i] = val-1;
                inf >> val;
                inf >> val;
                set1[i] = val;
            }
            points.push_back(set);
            facePoints.push_back(set1);
        }
    }
    inf.close();
}

void Model::draw(Matrix<4,4>  persp, Matrix<4,4> modelView, Matrix<4,4> vPort, int width, int height, double ambient, vec<3> light, vec<3> viewer){
    modelBuffer = TGAImage{width, height, TGAImage::GRAYSCALE};
    TGAImage grayBuffer = TGAImage(width, height, TGAImage::GRAYSCALE);

    vec<4> temp = {light[0], light[1], light[2], 0.};
    temp = modelView*temp;
    light = {temp[0], temp[1], temp[2]};
    light = light.normalized();

    int size = points.size();

    vec<3> coords[3];
    vec<4> clip[3];
    vec<3> Z{};
    vec<3> norms[3];
    for(int i = 0; i<points.size(); i++){
        std::vector<int> set = points[i], set1 = facePoints[i];
        for(int i:{0,1,2}){coords[i] = vertices[set[i]];}
        for(int i:{0,1,2}){norms[i] = normals[set1[i]];}
        for(int i = 0; i < 3; i++){ 
            clip[i] = modelView * vec<4>{coords[i].x, coords[i].y, coords[i].z, 1.};
            norms[i] = {clip[i][0], clip[i][1], clip[i][2]};
            clip[i] = persp * clip[i];
            clip[i] = (clip[i]/clip[i][3]);
            clip[i] = vPort*clip[i];
            Z[i] = clip[i][2];
        }
        
        vec<2> args[3];
        for(int i = 0; i< 3; ++i){
            args[i] = vec<2>{clip[i][0], clip[i][1]};
        }
        draw::rasterize(args, Z, norms, ambient, modelBuffer, grayBuffer, light, viewer);
    }
    grayBuffer.write_tga_file("grayBuff1.tga");
}

void Model::write(std::string filename){
    this->modelBuffer.write_tga_file(filename);
    return;
}