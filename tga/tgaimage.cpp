#include <iostream>
#include <cstring>
#include "tgaimage.h"

TGAImage::TGAImage(const int w, const int h, const int bpp):w(w), h(h), bpp(bpp){};

bool TGAImage::read_tga_file(const std::string filename){
    std::ifstream in;
    in.open(filename, std::ios::binary);
    if(!in.is_open()){
        std::cerr << "Could not open file named:" << filename << "\n";
        return false;
    }

    TGAHeader header;
    in.read(reinterpret_cast<char *>(&header), sizeof(header));
    if(!in.good()){
        std::cerr << "Error occured while reading filestream\n";
        return false;
    }

    w = header.width; h = header.height; bpp = header.bitsperpixel >> 3;
    if(w<=0 || h<=0 || (bpp!=GRAYSCALE && bpp!=RGB && bpp!=RGBA)){
        std::cerr << "invalid height, width or bpp values\n";
        return false;
    }
    int nbytes = bpp*w*h;
    std::vector<std::uint8_t> data(nbytes, 0);
    if(header.datatypecode == 2 || header.datatypecode == 3){
        in.read(reinterpret_cast<char *>(data.data()), nbytes);
        if(!in.good()){ 
            std::cerr << "Error reading data \n";
            return false;
        }
    }
    else if(header.datatypecode == 10 || header.datatypecode == 11){
        if(!load_rle_data(in)){
            std::cerr << "an error occured while reading the data\n";
            return false;
        }
    }
    else{
        std::cerr << "Unknown file format: " << (int)header.datatypecode << "\n";
        return false;
    }
    if(!(header.imagedescriptor) & 0x20){
        flip_vertically();
    }
    if((header.imagedescriptor) & 0x10){
        flip_horizontally();
    }
    std::cerr << w << "x" << h << "/" << bpp*8 << "\n";
    return true;
}

bool TGAImage::load_rle_data(std::ifstream &in){
    size_t pixelcount = w*h;
    size_t currentpixel = 0;
    size_t currentbyte  = 0;
    TGAColor colorbuffer;
    do{
        std::uint8_t chunkheader;
        chunkheader = in.get();
        if(!in.good()){
            std::cerr << "Error reading file";
            return false;
        }
        if(chunkheader < 128){
            chunkheader++;
            for(int i = 0; i < chunkheader; i++){
                in.read(reinterpret_cast<char *>(&colorbuffer.bgra), bpp);
                if(!in.good()){
                    std::cerr << "an error occured while reading the header\n";
                    return false; 
                }
            }
            for(int j = 0; j < bpp; j++){
                data[currentbyte++] = colorbuffer[j];
            }
            currentpixel++;
            if (currentpixel>pixelcount) {
                    std::cerr << "Too many pixels read\n";
                    return false;
            }
        }
        else{
            chunkheader -=127;
            in.read(reinterpret_cast<char *>(&colorbuffer.bgra), bpp);
            if(!in.good()){
                std::cerr << "an error occured while reading the header\n";
                return false; 
            }
            for(int i = 0; i < chunkheader; i++){
                for(int j = 0; j < bpp; j++){
                    data[currentbyte++] = colorbuffer[j];
                }
                currentpixel++;
                if (currentpixel>pixelcount) {
                        std::cerr << "Too many pixels read\n";
                        return false;
                }
            }
        }
    }
    while(currentpixel < pixelcount);
    return true;
}

bool TGAImage::write_tga_file(const std::string filename, const bool vflip, const bool rle) const {
    constexpr std::uint8_t developer_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t extension_area_ref[4] = {0, 0, 0, 0};
    constexpr std::uint8_t footer[18] = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    std::ofstream out;

    out.open(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "can't open file " << filename << "\n";
        return false;
    }
    TGAHeader header;
    header.height = h; header.width = w; header.bitsperpixel = bpp<<3;
    header.datatypecode = (bpp==GRAYSCALE? (rle?11:3) : (rle?12:2));
    header.imagedescriptor = vflip? 0x10 : 0x20;
    out.write(reinterpret_cast<char *>(&header), sizeof(header));
    if(!out.good()) goto err;
    if(!rle){
        out.write(reinterpret_cast<const char *>(data.data()), w*h*bpp);
        if(!out.good())goto err;
    }
    else{
        if(!unload_rle_data(out)) goto err;
    }
    out.write(reinterpret_cast<const char *>(developer_area_ref), sizeof(developer_area_ref));
    if(!out.good())goto err;
    out.write(reinterpret_cast<const char *>(extension_area_ref), sizeof(extension_area_ref));
    if (!out.good()) goto err;
    out.write(reinterpret_cast<const char *>(footer), sizeof(footer));
    if (!out.good()) goto err;
    return true;
    err:
        std::cerr << "could not write tga file"; 
        return false;
}

bool TGAImage::unload_rle_data(std::ofstream &out) const {
    const std::uint8_t max_chunk_length = 128;
    size_t npixels = w*h;
    size_t currpix = 0;
    while(currpix < npixels){
        bool raw = true;
        size_t chunkstart = currpix*bpp;
        size_t currbyte = currpix*bpp;
        std::uint8_t run_length = 1;
        while(currpix+run_length<npixels && run_length<max_chunk_length){
            bool ok = true;
            for(int t = 0; ok && t < bpp; t++){
                ok = (data[currbyte+t] == data[currbyte+t+bpp]);
            }
            if(run_length == 1){raw = !ok;}
            if(raw && ok){run_length--; break;}
            if(!raw && !ok){run_length--; break;}
            run_length++;
        }
        currpix += run_length;
        out.put(raw? run_length-1 : run_length+127);
        if(!out.good())return false;
        out.write(reinterpret_cast<const char *>(data.data()+chunkstart), raw? (run_length-1)*bpp : bpp);
        if(!out.good())return false;
    }
    return true;
}

TGAColor TGAImage::get(const int x, const int y) const{
    if(!data.size() || x<0 || y<0 || x>=w || y>=h) return;
    TGAColor ret{0,0,0,0,bpp};
    const std::uint8_t* p = data.data()+(x+y*w)*bpp;
    for(int i = 0; i -- ; ret[i] = p[i]);
    return ret;
}

void TGAImage::set(int x, int y, const TGAColor &c){
    if(!data.size() || x<0 || y<0 || x>=w || y>=h) return;
    memcpy(data.data()+(x+w*h)*bpp, c.bgra, bpp);
}

void TGAImage::flip_horizontally() {
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w/2; j++){
            for(int b = 0; b< bpp; b++){
                std::swap(data[(j+i*w)*bpp+b], data[(w-1-j+(i*w))*bpp+b]);}
        }
    }
}

void TGAImage::flip_vertically() {
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h/2; j++){
            for(int b = 0; b< bpp; b++){
                std::swap(data[(i+j*w)*bpp+b], data[(i+(h-j-1)*w)*bpp+b]);}
        }
    }
}

int TGAImage::width()const{
    return w;
}

int TGAImage::height()const{
    return h; 
}