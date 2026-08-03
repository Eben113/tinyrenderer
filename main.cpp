#include <cmath>
#include <chrono>
#include <iostream>
#include "tgaimage.h"


constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void nline(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color){
    bool steep = false;
    int dy = by - ay, dx = bx - ax;
    int E = 2*dy - dx, y = ay;

    if(dy > dx){
        steep = true;
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if(ax > bx){
        std::swap(ax, bx);
        std::swap(ay, by);
    }

    for(int x = ax; x < bx; x++){
        if(steep){framebuffer.set(y, x, color);}
        else{framebuffer.set(x, y, color);}
        if(E < 0){
            E += 2*dy; 
            y+= 1;}
        else{E += 2*(dy - dx);}

    }
}


int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::srand(std::time({}));
    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        nline(ax, ay, bx, by, framebuffer, {255, 255, 255, 255});
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration<double>(end - start).count() << '\n';

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}