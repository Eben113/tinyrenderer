#include <string>
#include "tgaimage.h"


class Model{
    public:
        Model(std::string filename, TGAColor color, int width, int height);
        void write(std::string filename);

    private:
        TGAImage modelBuffer;
};