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

int main(int argc, char** argv) {
    constexpr int width  = 640;
    constexpr int height = 640;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model diablo{"obj/diablo3_pose/diablo3_pose.obj", width, height};
    diablo.draw(red, width, height, 0,0,0);
    std::cout << "done\n";
    diablo.write("diablo90_10.tga");
    
    return 0;
}