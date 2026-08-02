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

void Model::setViewport(int x, int y, int w, int h){
    this->viewport = Matrix<4,4>{{{w/2,0,0,x+w/2}, {0,h/2,0,y+h/2}, {0,0,1,0}, {0,0,0,1}}};
}

void Model::setPerspective(const double f){                                
    this->perspective = Matrix<4,4>{{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,-1/f,1}}};
}

void Model::setModelView(const vec<3> c, const vec<3> eye, vec<3> up){
    vec<3> n = (eye - c).normalized();
    vec<3> l = (up.cross(n)).normalized();
    vec<3> m = (n.cross(l)).normalized(); 
    this->modelView =  Matrix<4,4>{{{l.x, l.y, l.z, 0}, {m.x, m.y, m.z, 0}, {n.x, n.y, n.z, 0}, {0,0,0,1}}}* Matrix<4,4>{{{1,0,0,-c.x}, {0,1,0,-c.y}, {0,0,1,-c.z}, {0,0,0,1}}};
}


void Model::setLight(vec<3> light){
    vec<4> temp = {light[0], light[1], light[2], 0.};
    temp = modelView*temp;
    this->light = {temp[0], temp[1], temp[2]};
    this->light = light.normalized();
}

vec<3> Model::cameraTransform(vec<3> coord){
    vec<4> clip;
    clip = modelView * vec<4>{coord.x, coord.y, coord.z, 1.};
    clip = perspective * clip;
    clip = (clip/clip[3]);
    clip = viewport*clip;
    return {clip[0], clip[1], clip[2]};;

}


Model::Model(std::vector<std::string> filenames, std::vector<std::string> nmFiles, std::vector<std::string> tang_nmFiles, std::vector<std::string> diffFiles, std::vector<std::string> specFiles){
    vertices.resize(filenames.size());
    normals.resize(filenames.size());
    uvs.resize(filenames.size());
    vertsIdx.resize(filenames.size());
    uvIdx.resize(filenames.size());
    normIdx.resize(filenames.size());
    
    for(int i = 0; i < filenames.size(); ++i){
        std::string filename = filenames[i], nmFile = nmFiles[i],tang_nmFile = tang_nmFiles[i], diffFile = diffFiles[i], specFile = specFiles[i];

        std::ifstream inf{filename};
        std::string line;

        vec<3> point;
        std::vector<int> set(3, 0);
        std::vector<int> set1(3, 0);
        std::vector<int> set2(3, 0);
        int val;
        std::cout << "populating data" << "\n";
        while(inf >> line){
            if(line == "v"){
                float coord;
                for(int i = 0; i<3; i++){
                    inf >> coord;
                    point[i] = coord;
                }
                this->vertices[i].push_back(point);
            }

            else if(line == "vn"){
                float coord;
                for(int i = 0; i<3; i++){
                    inf >> coord;
                    point[i] = coord;
                }
                normals[i].push_back(point);
            }

            else if(line == "vt"){
                float coord;
                for(int i = 0; i<3; i++){
                    inf >> coord;
                    point[i] = coord;
                }
                this->uvs[i].push_back(point);
            }

            else if(line == "f"){
                std::string discard;
                for(int i = 0; i<3; i++){
                    inf >> val;
                    set[i] = val-1;
                    inf.seekg(1, std::ios::cur);
                    inf >> val;
                    set1[i] = val-1;
                    inf.seekg(1, std::ios::cur);
                    inf >> val;
                    set2[i] = val-1;
                }
                this->vertsIdx[i].push_back(set);
                this->uvIdx[i].push_back(set1);
                this->normIdx[i].push_back(set2);
            }
        }
        if(tang_nmFile != ""){
            tang_nmBuffer.read_tga_file(tang_nmFile);
        }

        if(diffFile != ""){
            diffBuffer.read_tga_file(diffFile);
        }

        if(specFile != ""){
            specBuffer.read_tga_file(specFile);
        }

        std::cout << vertsIdx[i].size() << "  " << uvIdx[i].size() << "\n";
        inf.close();
    }
    std::cout << "done\n";
}

void Model::draw(TGAImage& frameBuffer, std::vector<double>& zBuffer){
    int ind  = 0;
    for(int i = 0; i < vertsIdx.size(); ++i){
        int size = vertsIdx[i].size();

        vec<3> coords[3] = {vec<3>{}};
        vec<3> Z{};
        vec<3> norms[3] = {vec<3>{}};
        vec<3> UV[3] = {vec<3>{}};
        vec<2> args[3] = {vec<2>{}};

        Matrix<3,2> E;
        Matrix<2,2> U;
        Matrix<3,3> TBN;

        Matrix<2,3> temp;

        for(int j = 0; j<size; j++){
            std::vector<int> set = vertsIdx[i][j], set1 = uvIdx[i][j], set2 = normIdx[i][j];;

            for(int k:{0,1,2}){
                coords[k] = vertices[i][set[k]];
                UV[k] = uvs[i][set1[k]];
                norms[k] = normals[i][set2[k]];
            }

            temp = {coords[0]-coords[1], coords[2]-coords[1]};
            E = temp.T();

            temp = {UV[0]-UV[1], UV[2]-UV[1]};
            U[0] = {temp[0][0], temp[1][0]}; U[1] = {temp[0][1], temp[1][1]};
            temp = (E*(U.inverse())).T();
            
            TBN [1] = {temp[0][0], temp[1][0], temp[2][0]};
            TBN[2] = {temp[0][1], temp[1][1], temp[2][1]};

            for(int k = 0; k < 3; k++){ 
                coords[k]  = cameraTransform(coords[k]);
            }
            for(int k = 0; k< 3; ++k){
                args[k] = vec<2>{coords[k][0], coords[k][1]};
                Z[k] = coords[k][2];
            }
            draw::rasterize(args, Z, UV, ambient, modelView, light, TBN, norms, frameBuffer, tang_nmBuffer, zBuffer, diffBuffer, specBuffer);
        }
    }
    this->modelBuffer = frameBuffer;
}

void Model::write(std::string filename){
    this->modelBuffer.write_tga_file(filename);
    return;
}Commit on 2026-06-18T12:00:00
Commit on 2026-06-19T12:00:00
Commit on 2026-06-25T12:00:00
Commit on 2026-07-07T12:00:00
Commit on 2026-07-21T12:00:00
Commit on 2026-07-23T12:00:00
Commit on 2026-07-29T12:00:00
Commit on 2026-07-10T12:00:00
Commit on 2026-07-14T12:00:00
Commit on 2026-07-18T12:00:00
Commit on 2026-07-26T12:00:00
Commit on 2026-08-19T12:00:00
Commit on 2026-08-21T12:00:00
Commit on 2026-08-23T12:00:00
Commit on 2026-08-02T12:00:00
