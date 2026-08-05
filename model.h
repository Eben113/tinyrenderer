#include <string>
#include "tgaimage.h"
#include <vector>
#include "linAlg.h"


class Model{
    public:
        Model(std::string filename, int width, int height);
        void write(std::string filename);
        void draw(TGAColor color, int width, int height, float thetaX, float thetaY, float thetaZ);

    private:
        TGAImage modelBuffer;
        std::vector<vec<3>> vertices;
        std::vector<vec<3>> points;
};