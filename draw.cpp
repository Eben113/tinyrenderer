#include "draw.h"
#include <iostream>
#include <array>
#include "linAlg.h"


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

int triangleArea(vec<2> p0, vec<2> p1, vec<2> p2){
    return   ((p1[0]-p0[0])*(p2[1]-p0[1])) - ((p1[1]-p0[1])*(p2[0]-p0[0]));
}
int triangleArea(int x0, int y0, int x1, int y1, int x2, int y2){
    return   ((x1-x0)*(y2-y0)) - ((y1-y0)*(x2-x0));
}

void triangle(vec<2> p0, vec<2> p1, vec<2> p2, TGAImage &framebuffer, TGAColor color){
    int totalArea = triangleArea(p0, p1,  p2);
    if(triangleArea(p0, p1, p2) <= 1){
        return;}
    int orientation = (totalArea < 0)? -1: 1;
    int maxX = std::max(std::max(p0[0], p1[0]), p2[0]), minX = std::min(std::min(p0[0], p1[0]), p2[0]);
    int maxY = std::max(std::max(p0[1], p1[1]), p2[1]), minY = std::min(std::min(p0[1], p1[1]), p2[1]);

    int alpha, beta, gamma;
    for(int i = minX; i<=maxX; i++){ 
        for(int j = minY; j<=maxY; j++){
            vec<2> p = {i, j};
            alpha = triangleArea(p, p1, p2)*orientation;
            beta = triangleArea(p0, p, p2)*orientation;
            gamma = triangleArea(p0, p1, p)*orientation;

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

void rasterize(const vec<2> screen[3], vec<3> z, vec<3> norms[3], double ambient, TGAImage &framebuffer, TGAImage& grayBuffer, vec<3> l, vec<3> viewer){
    int totalArea = triangleArea(screen[0], screen[1], screen[2]);
    int orientation = (totalArea < 0)? -1: 1;
    int absArea = orientation*totalArea;
    if(totalArea <= 0){
        return;}
    Matrix<3,3> ABC {};

    vec<3> n, r;
    double diffuse, specular;

    for(int i = 0; i< 3; ++i){
        ABC[i] = vec<3>{screen[i][0], screen[i][1], 1};
    }
    ABC = ABC.transposeInverse();
    int maxX = std::max(std::max(screen[0][0], screen[1][0]), screen[2][0]), minX = std::min(std::min(screen[0][0], screen[1][0]), screen[2][0]);
    int maxY = std::max(std::max(screen[0][1], screen[1][1]), screen[2][1]), minY = std::min(std::min(screen[0][1], screen[1][1]), screen[2][1]);
    #pragma op parallel for
    for(int i = minX; i<=maxX; i++){ 
        for(int j = minY; j<=maxY; j++){
            vec<2> p {(double)i, (double)j};
            vec<3> bc = {triangleArea(screen[0], screen[1], p), triangleArea(p, screen[1], screen[2]),
            triangleArea(screen[0], p, screen[2])};
            bc = (bc/totalArea);
            
            if((bc[0] >= 0) && (bc[1] >= 0) && (bc[2] >= 0)){
                n = (bc[0]*norms[0] + bc[1]*norms[1] + bc[2]*norms[2]).normalized();
                //std::cout << n;
                r = (n*(n.dot(l)*2) - l).normalized();
                diffuse = std::max(0., n.dot(l));
                specular = std::pow(std::max(0., r.z), 35);
                
                TGAColor color {255, 255, 255, 255};
                for(int i : {0,1,2}){color[i] *= std::min(1., ambient + .4*diffuse + .9*specular);}
            
                uint8_t depthColor = ((bc/totalArea).dot(z))*1.275;
                if((int)grayBuffer.get(i,j).bgra[0] <= depthColor){
                    grayBuffer.set(i, j, {depthColor, depthColor, depthColor});
                    framebuffer.set(i, j, color);
                }
        }
        }
    }
}
}