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
vec<3> viewer = (eye-center).normalized();


int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::vector<double> zBuff(width*height, -INFINITY);

    Model diablo({"obj/diablo3_pose/diablo3_pose.obj"}, {"obj/diablo3_pose/diablo3_pose_nm.tga"}, 
        {"obj/diablo3_pose/diablo3_pose_nm_tangent.tga"},
    {"obj/diablo3_pose/diablo3_pose_diffuse.tga"}, {"obj/diablo3_pose/diablo3_pose_spec.tga"});
    
    diablo.setViewport(width/16, height/16, width*7/8, height*7/8);
    diablo.setPerspective((eye-center).mag());
    diablo.setModelView(center, eye, up);

    diablo.setLight({1,0.6,1});
    diablo.setAmbient(0.08);

    auto start = std::chrono::steady_clock::now();
    diablo.draw(framebuffer, zBuff);
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "done in " << duration.count() << "\n";
    framebuffer.write_tga_file("diablo90_10.tga");
    
    return 0;
}