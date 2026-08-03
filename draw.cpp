#include "draw.h"
#include <iostream>
#include <array>


namespace draw{

constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};



void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw_, std::vector<std::array<int,2>>* points){
    if(ay == by){
        if(ax > bx){std::swap(ax,bx);}
        for(int x = ax; x<=bx; x++){framebuffer.set(x, ay, color);}
        return;
    }
    bool steep = false;
    int dy = abs(by - ay), dx = abs(bx - ax);

    if(dy > dx){
        steep = true;
        std::swap(ax, ay);
        std::swap(bx, by);
        std::swap(dx, dy);
    }

    if(ax > bx){
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int step = by<ay?-1:1;

    int E = 0, y = ay;
    if(!draw_){
        std::vector<std::array<int,2>>& res = *points;

        for(int x = ax; x <= bx; x++){
            if(steep){
                std::array<int,2> p = {y,x};
                res.push_back(p);
            }
            else{
                std::array<int,2> p = {x,y};
                res.push_back(p);
            }
            E += 2*dy -  2*dx*(E>0);
            y += step*(E>0);
        }
    }

    else{
        for(int x = ax; x <= bx; x++){
            if(steep){framebuffer.set(y, x, color);}
            else{framebuffer.set(x, y, color);}

            E += 2*dy -  2*dx*(E>0);
            y += step*(E>0);
        }
    }

}

int triangleArea(int x0, int y0, int x1, int y1, int x2, int y2){
    return   ((x1-x0)*(y2-y0)) - ((y1-y0)*(x2-x0));
}

void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, TGAColor color){
    int totalArea = triangleArea(x0, y0, x1, y1, x2, y2);
    if(triangleArea(x0, y0, x1, y1, x2, y2) <= 1){
        return;}
    int orientation = (totalArea < 0)? -1: 1;
    int maxX = std::max(std::max(x0, x1), x2), minX = std::min(std::min(x0, x1), x2);
    int maxY = std::max(std::max(y0, y1), y2), minY = std::min(std::min(y0, y1), y2);

    int alpha, beta, gamma;
    for(int i = minX; i<=maxX; i++){ 
        for(int j = minY; j<=maxY; j++){
            alpha = triangleArea(i, j, x1, y1, x2, y2)*orientation;
            beta = triangleArea(x0, y0, i, j, x2, y2)*orientation;
            gamma = triangleArea(x0, y0, x1, y1, i, j)*orientation;

            if((alpha > 0) && (beta > 0) && (gamma > 0)){
                framebuffer.set(i, j, color);
            }
        }
    }
}

void gradientTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer){
    double totalArea = triangleArea(x0, y0, x1, y1, x2, y2);
    int orientation = (totalArea < 0)? -1: 1;
    
    int maxX = std::max(std::max(x0, x1), x2), minX = std::min(std::min(x0, x1), x2);
    int maxY = std::max(std::max(y0, y1), y2), minY = std::min(std::min(y0, y1), y2);

    int alpha, beta, gamma;
    for(int i = minX; i<=maxX; i++){ 
        for(int j = minY; j<=maxY; j++){
            alpha = triangleArea(i, j, x1, y1, x2, y2)*orientation;
            beta = triangleArea(x0, y0, i, j, x2, y2)*orientation;
            gamma = triangleArea(x0, y0, x1, y1, i, j)*orientation;

            uint8_t red = (alpha/abs(totalArea))*255, blue = (beta/abs(totalArea))*255, green = (gamma/abs(totalArea))*255;
            if((alpha > 0) && (beta > 0) && (gamma > 0)){
                TGAColor color {red, blue, green, 255};
                framebuffer.set(i, j, color);
            }
        }
    }
}


void wireframe(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, int thickness){
    double a01 = std::sqrt(std::pow((y1-y0),2) + std::pow((x1-x0),2))*thickness;
    double a02 = std::sqrt(std::pow((y2-y0),2) + std::pow((x2-x0),2))*thickness;
    double a12 = std::sqrt(std::pow((y2-y1),2) + std::pow((x2-x1),2))*thickness;


    std::cout << a01 << "  " << a02 << "  " << a12 << "\n";
    double totalArea = triangleArea(x0, y0, x1, y1, x2, y2);
    int orientation = (totalArea < 0)? -1: 1;
    
    int maxX = std::max(std::max(x0, x1), x2), minX = std::min(std::min(x0, x1), x2);
    int maxY = std::max(std::max(y0, y1), y2), minY = std::min(std::min(y0, y1), y2);

    int alpha, beta, gamma;
    for(int i = minX; i<=maxX; i++){ 
        for(int j = minY; j<=maxY; j++){
            alpha = triangleArea(i, j, x1, y1, x2, y2)*orientation;
            beta = triangleArea(x0, y0, i, j, x2, y2)*orientation;
            gamma = triangleArea(x0, y0, x1, y1, i, j)*orientation;

            uint8_t red = (alpha/abs(totalArea))*255, blue = (beta/abs(totalArea))*255, green = (gamma/abs(totalArea))*255;
            
            if((alpha > 0) && (beta > 0) && (gamma > 0)){
                if(alpha<=a12 || beta<=a02 || gamma<=a01){
                    TGAColor color {red, blue, green, 255};
                    framebuffer.set(i, j, color);
                }
            }
        }
    }
}

void depthTriangle(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, TGAColor color, TGAImage &framebuffer, TGAImage& grayBuffer){
    int totalArea = triangleArea(x0, y0, x1, y1, x2, y2);
    if(totalArea <= 0){
        return;}
    int orientation = (totalArea < 0)? -1: 1;
    int maxX = std::max(std::max(x0, x1), x2), minX = std::min(std::min(x0, x1), x2);
    int maxY = std::max(std::max(y0, y1), y2), minY = std::min(std::min(y0, y1), y2);
    double alpha, beta, gamma;
    for(int i = minX; i<=maxX; i++){ 
        for(int j = minY; j<=maxY; j++){
            alpha = triangleArea(i, j, x1, y1, x2, y2)*orientation;
            beta = triangleArea(x0, y0, i, j, x2, y2)*orientation;
            gamma = triangleArea(x0, y0, x1, y1, i, j)*orientation;

            if((alpha >= 0) && (beta >= 0) && (gamma >= 0)){
                uint8_t depthColor = ((alpha/totalArea)*z0 + (beta/totalArea)*z1 + (gamma/totalArea)*z2)*1.275;
                if((int)grayBuffer.get(i,j).bgra[0] <= depthColor){
                    grayBuffer.set(i, j, {depthColor, depthColor, depthColor});
                    framebuffer.set(i, j, color);
            }
        }
        }
    }
}
}