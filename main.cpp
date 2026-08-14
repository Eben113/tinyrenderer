#include <cmath>
#include <chrono>
#include <iostream>
#include "tgaimage.h"
#include "model.h"


constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

constexpr int width  = 800;    // output image size
constexpr int height = 800;
constexpr vec<3>    eye{-1,0,2};
constexpr vec<3> center{0,0,0};
constexpr vec<3>     up{0,1,0};

Matrix<4,4> modelView = lookAt(center, eye, up);
Matrix<4,4> persp = perspective((eye-center).mag());
Matrix<4,4> vPort = viewport(width/16, height/16, width*7/8, height*7/8);

int main(int argc, char** argv) {
    constexpr int width  = 640;
    constexpr int height = 640;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model diablo{"obj/diablo3_pose/diablo3_pose.obj", width, height};
    diablo.draw(persp, modelView, vPort, width, height);
    std::cout << "done\n";
    diablo.write("diablo90_10.tga");
    
    return 0;
}