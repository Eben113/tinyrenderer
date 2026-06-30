#include "draw.h"
#include <iostream>
#include <array>


namespace draw{
void swap(int& a, int& b){
    int temp{a};
    a = b; b = temp;
}


void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw, std::vector<std::array<int,2>>* points){
    bool steep = false;
    int dy = abs(by - ay), dx = abs(bx - ax);

    if(dy > dx){
        steep = true;
        swap(ax, ay);
        swap(bx, by);
        swap(dx, dy);
    }

    if(ax > bx){
        swap(ax, bx);
        swap(ay, by);
    }
    int step = by<ay?-1:1;

    int E = 0, y = ay;
    std::vector<std::array<int,2>>& res = *points;

    for(int x = ax; x < bx; x++){
        if(!draw){

            std::cout << res.size() << "\n";
            if(steep){
                std::array<int,2> p = {y,x};
                res.push_back(p);
            }
            else{
                std::array<int,2> p = {x,y};
                res.push_back(p);
            }
            std::cout << "here" << "\n";
        }

        else{
            if(steep){framebuffer.set(y, x, color);}
            else{framebuffer.set(x, y, color);}
        }

            E += 2*dy -  2*dx*(E>0);
            y += step*(E>0);

    }

}


void triangle(int x1, int x2, int x3, int y1, int y2, int y3, TGAImage &framebuffer, TGAColor color){
    int x[3] = {x1, x2, x3};
    int y[3] = {y1, y2, y3};


    for(int i = 0; i<2; i++){
        if(x[i] > x[i+1]){
            swap(x[i], x[i+1]);
            swap(y[i], y[i+1]);
        }
    }
    std::cout << x[0] << "  " << x[1] << "  " << x[2] << "\n";


    std::vector<std::array<int, 2>> line1, line2;

    line(x[0], y[0], x[1], y[1], framebuffer, color, false, &line1);
    line(x[1], y[1], x[2], y[2], framebuffer, color, false, &line2);
    std::cout << line1.size() << "  " << line2.size() << "\n";
    
    int i = 0;
    std::array<int,2> p1;
    std::array<int,2> p2;
    
    while((i<line1.size()) || (i<line2.size())){

        if(i<line1.size()){p1 = line1[line1.size()-i-1];}
        if(i<line2.size()){p2 = line2[line2.size()-i-1];}
        std::cout << p1[0] << "  " << p1[1] << "  " << p2[0] << "  " << p2[1] << "\n";
        line(p1[0], p1[1], p2[0], p2[1], framebuffer, color);
        i++;

    }
}

}