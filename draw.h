#include "tgaimage.h"
#include "linAlg.h"

namespace draw{
    void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw = true, std::vector<std::array<int,2>>* points = nullptr);
    int triangleArea(int x0, int y0, int x1, int y1, int x2, int y2);
    void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, TGAColor color);
    void gradientTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer);
    void wireframe(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, int thickness);
    void rasterize(const vec<2> screen[3], vec<3> z, vec<3> norms[3], double ambient, Matrix<4,4>& modelView, vec<3> l, Matrix<3,3> TBN, vec<3> normals[3], TGAImage &framebuffer, TGAImage& tang_nmBuffer, std::vector<double> &zBuffer, TGAImage& diffBuffer, TGAImage& specBuffer);
    void nline(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color);

}
