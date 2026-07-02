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
    
    TGAImage framebuffer = TGAImage(300, 300, TGAImage::RGB);
    draw::line(30, 30, 60, 60, framebuffer, blue);
    draw::triangle(30, 45, 100, 100, 120,  60, framebuffer, red);

    framebuffer.set(30, 100, white);
    framebuffer.set(45, 120, white);
    framebuffer.set(100, 60, white);

    framebuffer.write_tga_file("triangle.tga");
    return 0;
}