#include "draw.h"
#include <iostream>
#include <array>


namespace draw{
void swap(int& a, int& b){
    int temp{a};
    a = b; b = temp;
}


void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw, std::vector<std::array<int,2>>* points){
    if(ay == by){
        if(ax > bx){swap(ax,bx);}
        for(int x = ax; x<=bx; x++){framebuffer.set(x, ay, color);}
        return;
    }
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

void drawVerts(std::vector<std::array<int, 2>>& l1, std::vector<std::array<int, 2>>& l2, int start, TGAImage &framebuffer, TGAColor color){
    int step1 = l1[0][1] == start? 1 : -1;
    int step2 = l2[0][1] == start? 1: -1;
    int stop1 = step1 == 1? l1.size() - 1 : 0;
    int stop2 = step2 == 1? l2.size() - 1 : 0;
    int pointer1 = (step1 == 1)? 0: l1.size()-1, pointer2 = (step2 == 1)? 0: l2.size()-1;
    int height = std::abs(l1[0][1] - l1[l1.size()-1][1]);

    for(int y = start; y< start+height; y++){
        while((l1[pointer1][1] == y) && (pointer1 != stop1)){pointer1 += step1;}
        while((l2[pointer2][1] == y) && (pointer2 != stop2) ){pointer2 += step2;}
        std::cout << pointer1 << "  " << pointer2 << "\n";
        draw::line(l1[pointer1-step1][0], y, l2[pointer2-step2][0], y, framebuffer, color);
    }
}

void triangle(int x0, int x1, int x2, int y0, int y1, int y2, TGAImage &framebuffer, TGAColor color){
    if(y0>y1){swap(x0, x1); swap(y0, y1);}
    if(y0>y2){swap(x0, x2); swap(y0, y2);}
    if(y1>y2){swap(x1, x2); swap(y1, y2);}



    std::vector<std::array<int, 2>> line0, line1, line2, line3;

    line(x0, y0, x1, y1, framebuffer, color, false, &line0);
    line(x0, y0, x2, y2, framebuffer, color, false, &line1);
    line(x1, y1, x2, y2, framebuffer, color, false, &line2);

    line(x0, y0, x1, y1, framebuffer, color);
    line(x0, y0, x2, y2, framebuffer, color);
    line(x1, y1, x2, y2, framebuffer, color);

    int index = 0;
    for(auto [x,y] : line1){
        if(y != y1){index ++;}
        else{break;}
    }
    line3 = std::vector<std::array<int, 2>>(line1.begin()+index, line1.end());
    line1.erase(line1.begin()+index, line1.end());
    if(line1[0][1] > line3[line3.size()-1][1]){std::swap(line1, line3);}

    drawVerts(line0, line1, y0, framebuffer, color);
    drawVerts(line2, line3, y1, framebuffer, color);
}

}