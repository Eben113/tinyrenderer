#include <cmath>
#include "tgaimage.h"
#include "draw.h"
#include "model.h"


constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};


int main(int argc, char** argv) {
    
    TGAImage framebuffer(200, 200, TGAImage::RGB);
    draw::triangle(  7, 45, 35, 100, 45,  60, framebuffer, red);
    draw::triangle(120, 35, 90,   5, 45, 110, framebuffer, white);
    draw::triangle(115, 83, 80,  90, 85, 120, framebuffer, green);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}