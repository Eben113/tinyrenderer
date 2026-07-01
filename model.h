#ifndef MODEL
#define MODEL

#include <string>
#include "tgaimage.h"
#include <vector>
#include "linAlg.h"


class Model{
    public:
        Model(std::vector<std::string> filenames, std::vector<std::string> nmFiles, std::vector<std::string> tang_nmFiles, std::vector<std::string> diffFiles, std::vector<std::string> specFiles);

        Matrix<4,4> viewport;
        Matrix<4,4> perspective;
        Matrix<4,4> modelView;

        double ambient = 0.1;
        vec<3> light = {0, 0, 1};


        void setViewport(int x, int y, int w, int h);
        void setPerspective(const double f);
        void setModelView(const vec<3> c, const vec<3> eye, vec<3> up);

        void setAmbient(double ambient){this->ambient = ambient;}
        void setLight(vec<3> light);
        
        vec<3> cameraTransform(vec<3> coords);
        
        void write(std::string filename);
        void draw(TGAImage& frameBuffer, std::vector<double>& zBuffer);

    private:
        TGAImage modelBuffer;
        TGAImage tang_nmBuffer;
        TGAImage diffBuffer;
        TGAImage specBuffer;

        std::vector<std::vector<vec<3>>> vertices;
        std::vector<std::vector<vec<3>>> normals;
        std::vector<std::vector<vec<3>>> uvs;

        std::vector<std::vector<std::vector<int>>> vertsIdx;
        std::vector<std::vector<std::vector<int>>> uvIdx;
        std::vector<std::vector<std::vector<int>>> normIdx;
};

#endifCommit on 2026-06-05T12:00:00
Commit on 2026-06-03T12:00:00
Commit on 2026-06-14T12:00:00
Commit on 2026-06-29T12:00:00
Commit on 2026-07-01T12:00:00
