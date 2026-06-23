#include <cmath>
#include "tgaimage.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void swap(int& a, int& b){
    int temp{a};
    a = b; b = temp;
}

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color){
    bool steep = false;
    int dy = by - ay, dx = bx - ax;
    int E = 2*dy - dx, y = ay;

    if(dy > dx){
        steep = true;
        swap(ax, ay);
        swap(bx, by);
    }

    if(ax > bx){
        swap(ax, bx);
        swap(ay, by);
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
    for (int i=0; i<(1<<24); i++) {
        int ax = rand()%width, ay = rand()%height;
        int bx = rand()%width, by = rand()%height;
        line(ax, ay, bx, by, framebuffer, {static_cast<unsigned char>(rand() % 255),
                                static_cast<unsigned char>(rand() % 255),
                                static_cast<unsigned char>(rand() % 255),
                            static_cast<unsigned char>(rand() % 255)});
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
// int main(int argc, char** argv) {
//     constexpr int width  = 64;
//     constexpr int height = 64;
//     TGAImage framebuffer(width, height, TGAImage::RGB);

//     int ax =  7, ay =  3;
//     int bx = 12, by = 37;
//     int cx = 62, cy = 53;

//     line(ax, ay, bx, by, framebuffer, blue);
//     line(cx, cy, bx, by, framebuffer, green);
//     line(cx, cy, ax, ay, framebuffer, yellow);
//     line(ax, ay, cx, cy, framebuffer, red);


//     framebuffer.set(ax, ay, white);
//     framebuffer.set(bx, by, white);
//     framebuffer.set(cx, cy, white);

//     framebuffer.write_tga_file("framebuffer.tga");
//     return 0;
// }