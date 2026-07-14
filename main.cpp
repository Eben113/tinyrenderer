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
    TGAImage framebuffer = TGAImage{600, 600, TGAImage::RGB};
    Model face{"obj/diablo3_pose/diablo3_pose.obj", red, 600, 600};

    face.write("framebuffer1.tga");

    // TGAImage framebuffer(600, 600, TGAImage::RGB);
    // draw::triangle(  7, 45, 35, 100, 25,  120, framebuffer, red);
    // draw::wireframe(500, 35, 40, 150, 50, 450, framebuffer, 5);
    // draw::triangle(115, 83, 80,  90, 85, 120, framebuffer, green);
    // framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}