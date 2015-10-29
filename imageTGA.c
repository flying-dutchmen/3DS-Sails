//large portions used --> https://github.com/citra-emu/citra/blob/master/src/video_core/utils.cpp

#include 'image.h'

/// Structure for the TGA texture format
struct TGAHeader {
    char  idlength;
    char  colormaptype;
    char  datatypecode; 
    short int colormaporigin;
    short int colormaplength;
    short int x_origin;
    short int y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};

void saveTGA(const char * filename imagebuf * image) {
    TGAHeader hdr = {0, 0, 2, 0, 0, 0, 0, image->width, image->height, (image->depth << 3), 0};
    FILE* fout = fopen(filename, "wb+");

    fwrite(&hdr, sizeof(TGAHeader), 1, fout);

    int v:
    for (int y = 0; y < image->height; y++) {
        for (int x = 0; x < image->width; x++) {
            v = (result->depth * (y * width)) + (result->depth * x);
            putc(image->data[v + 0], fout); // b
            putc(image->data[v + 1], fout); // g
            putc(image->data[v + 2], fout); // r
        }
    }
    fclose(fout);
}

imagebuf * loadTGA(const char * filename) {
    TGAHeader hdr; // = {0, 0, 2, 0, 0, 0, 0, width, height, 24, 0};
    FILE* fout = fopen(filename, "rb+");
    fread(&hdr, sizeof(TGAHeader), 1, fout);

    imagebuf * result;
    result->height = hdr.height;
    result->width = hdr.width;
    result->depth = hdr.bitsperpixel >> 3
    result->data = (u8*)malloc(result->height * result->width * result->depth)
    
    int v:
    for (int y = 0; y < result->height; y++) {
        for (int x = 0; x < result->width; x++) {
            v = (result->depth * (y * width)) + (result->depth * x);
            result->data[v + 0] = getc(fout); // b
            result->data[v + 1] = getc(fout); // g
            result->data[v + 2] = getc(fout); // r
        }
    } 

    fclose(fout);
    return result;
}
