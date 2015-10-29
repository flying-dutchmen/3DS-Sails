//large portions used --> https://github.com/citra-emu/citra/blob/master/src/video_core/utils.cpp

#include <3ds.h>
#include <stdio.h>
#include "image.h"

#include "Canvas_font.h"

/// Structure for the TGA texture format
typedef struct {
    u8  idlength;
    u8  colormaptype;
    u8  datatypecode; 
    u16 colormaporigin;
    u16 colormaplength;
//colourmapdepth craps on % of header gimp doesn't bother with & not mentioned --> https://en.wikipedia.org/wiki/Truevision_TGA
//    u8  colourmapdepth; -->  http://www.paulbourke.net/dataformats/tga/ 
    u16 x_origin;
    u16 y_origin;
    u16 width;
    u16 height;
    u8  bitsperpixel;
    u8  imagedescriptor;
} TGAHeader;

void TGAHeaderdebug(u8* screen, TGAHeader *image)
{
char* str[256];
sprintf(str, "idlength: %u", image->idlength);
CanvasString(screen, str, 0, 10, WHITE);
sprintf(str, "colormaptype %u \n", image->colormaptype);
CanvasString(screen, str, 0, 20, WHITE);
sprintf(str, "datatypecode %u \n", image->datatypecode);
CanvasString(screen, str, 0, 30, WHITE);
sprintf(str, "colormaporigin %u \n", image->colormaporigin);
CanvasString(screen, str, 0, 40, WHITE);
sprintf(str, "colormaplength %u", image->colormaplength);
//CanvasString(screen, str, 0, 50, WHITE);
//sprintf(str, "colourmapdepth %u", image->colourmapdepth);
CanvasString(screen, str, 0, 50, WHITE);
sprintf(str, "x_origin %u", image->x_origin);
CanvasString(screen, str, 0, 60, WHITE);
sprintf(str, "y_origin %u", image->y_origin);
CanvasString(screen, str, 0, 70, WHITE);
sprintf(str, "biXPelsPerMeter %u", image->width);
CanvasString(screen, str, 0, 80, WHITE);
sprintf(str, "biYPelsPerMeter %u", image->height);
CanvasString(screen, str, 0, 90, WHITE);
sprintf(str, "bitsperpixel %u", image->bitsperpixel);
CanvasString(screen, str, 0, 100, WHITE);
sprintf(str, "imagedescriptor %u", image->imagedescriptor);
CanvasString(screen, str, 0, 110, WHITE);
}

void saveTGA(const char * filename, imagebuff * image) {
    TGAHeader hdr = {0, 0, 2, 0, 0, 0, 0, image->width, image->height, (image->depth << 3), 0};
    FILE* fout = fopen(filename, "wb+");

    fwrite(&hdr, sizeof(TGAHeader), 1, fout);
    
    int v,x,y;
    for (y = 0; y < image->height; y++) {
        for (x = 0; x < image->width; x++) {
            v = (image->depth * (y * image->width)) + (image->depth * x);
            putc(image->data[v + 0], fout); // b
            putc(image->data[v + 1], fout); // g
            putc(image->data[v + 2], fout); // r
        }
    }
    fclose(fout);
}

imagebuff * loadTGA(const char * filename) {
    TGAHeader hdr; // = {0, 0, 2, 0, 0, 0, 0, width, height, 24, 0};
    FILE* fout = fopen(filename, "rb+");
    memset ( &hdr, 0, sizeof (TGAHeader ) );

    fread(&hdr, sizeof(TGAHeader), 1, fout);
    TGAHeaderdebug(screenBottom, &hdr);

    imagebuff * result;
    result->height = hdr.height;
    result->width = hdr.width;
    result->depth = hdr.bitsperpixel >> 3;

//char* str[256];
//sprintf(str, "result->height: %u", result->height);
//CanvasString(screenBottom, str, 0, 170, WHITE);

//sprintf(str, "result->width: %u", result->width);
//CanvasString(screenBottom, str, 0, 180, WHITE);

    result->data = (u8*) malloc(result->height * result->width * result->depth);
    memset(&result->data, 0, result->height * result->width * result->depth);

    int v,x,y;
    for (y = 0; y < result->height; y++) {
        for (x = 0; x < result->width; x++) {
            v = (result->depth * (y * result->width)) + (result->depth * x);
            result->data[v + 0] = fgetc(fout); // b
            result->data[v + 1] = fgetc(fout); // g
            result->data[v + 2] = fgetc(fout); // r
        }
    } 

    fclose(fout);
    return result;
}
