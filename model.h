#ifndef MODEL
#define MODEL

#include <string>
#include "tgaimage.h"
#include <vector>
#include "linAlg.h"


class Model{
    public:
        Model(std::string filename, int width, int height);
        void write(std::string filename);
        void draw(Matrix<4,4>  persp, Matrix<4,4> modelView, Matrix<4,4> vPort, int width, int height, double ambient, vec<3> light, vec<3> viewer);

    private:
        TGAImage modelBuffer;
        std::vector<vec<3>> vertices;
        std::vector<std::vector<int>> points;
};

Matrix<4,4> lookAt(const vec<3> c, const vec<3> eye, vec<3> up);
Matrix<4,4> perspective(const double f);
Matrix<4,4> viewport(int x, int y, int w, int h);
#endif