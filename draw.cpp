#include "draw.h"
#include <iostream>
#include <array>


namespace draw{

constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};



void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw, std::vector<std::array<int,2>>* points){
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
    std::vector<std::array<int,2>>& res = *points;

    for(int x = ax; x <= bx; x++){
        if(!draw){


            if(steep){
                std::array<int,2> p = {y,x};
                res.push_back(p);
            }
            else{
                std::array<int,2> p = {x,y};
                res.push_back(p);
            }
        }

        else{
            if(steep){framebuffer.set(y, x, color);}
            else{framebuffer.set(x, y, color);}
        }

            E += 2*dy -  2*dx*(E>0);
            y += step*(E>0);

    }

}

int triangleArea(int x0, int y0, int x1, int y1, int x2, int y2){
    return   ((y2 - y1)*(x0 - x1)) - ((y0 - y1)*(x2 - x1));
}

void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, TGAColor color){
    if(triangleArea(x1, y1, x0, y0, x2, y2) < 0){
        std::swap(x1, x2); std::swap(y1, y2);}
    
    int maxX = x0, minX = x0, maxY = y0, minY = y0;
    if(x1 > x0){maxX = x1;}
    else{minX = x1;}
    if(x2 > maxX){maxX = x2;}
    else if(x2 < minX){minX = x2;}

    if(y1 > y0){maxY = y1;}
    else{minY = y1;}
    if(y2 > maxY){maxY = y2;}
    else if(y2 < minY){minY = y2;}

    int alpha, beta, gamma;
    for(int i = minX; i<=maxX; i++){
        for(int j = minY; j<=maxY; j++){
            alpha = triangleArea(x0, y0, i, j, x1, y1);
            beta = triangleArea(x1, y1, i, j, x2, y2);
            gamma = triangleArea(x2, y2, i, j, x0, y0);

            if((alpha > 0) && (beta > 0) && (gamma > 0)){
                framebuffer.set(i, j, color);
            }
        }
    }
}

}