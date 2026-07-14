#include "tgaimage.h"

namespace draw{
    void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw = true, std::vector<std::array<int,2>>* points = nullptr);
    int triangleArea(int x0, int y0, int x1, int y1, int x2, int y2);
    void triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, TGAColor color);
    void gradientTriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer);
    void wireframe(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &framebuffer, int thickness);
    void depthTriangle(int x0, int y0, int z0, int x1, int y1, int z1, int x2, int y2, int z2, TGAColor color, TGAImage &framebuffer, TGAImage& grayBuffer);

}
