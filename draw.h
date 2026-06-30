#include "tgaimage.h"

namespace draw{
    void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color, bool draw = true, std::vector<std::array<int,2>>* points = nullptr);

    void triangle(int x1, int x2, int x3, int y1, int y2, int y3, TGAImage &framebuffer, TGAColor color);

}
