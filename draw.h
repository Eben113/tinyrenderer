#include "tgaimage.h"
#include "linAlg.h"

namespace draw{
    void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw = true, std::vector<std::array<int,2>>* points = nullptr);
    int triangleArea(int x0, int y0, int x1, int y1, int x2, int y2);
    void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, TGAColor color);
    void gradientTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer);
    void wireframe(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, int thickness);
    void rasterize(const vec<2> screen[3], vec<3> z, double ambient, TGAImage &framebuffer, TGAImage& grayBuffer, vec<3> sun, vec<3> viewer);
    void nline(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color);

}
