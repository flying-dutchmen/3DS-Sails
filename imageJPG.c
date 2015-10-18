#include <3ds.h>
#include <stdio.h>

#include "jpeglib.h"
#include "image.h"


//revised --> Rinnegatamante :: Lua-Player-Plus :: Graphics.cpp :: Bitmap* decodePNGfile(const char* filename);
imagebuff* loadJPG(const char* filename)
{
    imagebuff* result = (imagebuff*)malloc(sizeof(imagebuff));
	if (result == NULL) return 0;
	u64 size;
	u32 bytesRead;
	Handle fileHandle;
//        jpeg_std_error jerr;
    struct jpeg_decompress_struct cinfo;
//	struct my_error_mgr jerr;
 	cinfo.err = &jpeg_std_error;
 //    jerr.pub.error_exit = my_error_exit;
    jpeg_create_decompress(&cinfo);
	FS_path filePath = FS_makePath(PATH_CHAR, filename);
	FS_archive archive = (FS_archive) { ARCH_SDMC, (FS_path) { PATH_EMPTY, 1, (u8*)"" }};
	FSUSER_OpenFileDirectly(NULL, &fileHandle, archive, filePath, FS_OPEN_READ, FS_ATTRIBUTE_NONE);	
	FSFILE_GetSize(fileHandle, &size);
	unsigned char* in = (unsigned char*)malloc(size);
	if(!in) {
		FSFILE_Close(fileHandle);
		svcCloseHandle(fileHandle);
		return 0;
	}
	FSFILE_Read(fileHandle, &bytesRead, 0x00, in, size);
	FSFILE_Close(fileHandle);
	svcCloseHandle(fileHandle);
    jpeg_mem_src(&cinfo, in, size);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);
    int width = cinfo.output_width;
    int height = cinfo.output_height;
    int row_bytes = width * cinfo.num_components;
    u8* bgr_buffer = (u8*) malloc(width*height*cinfo.num_components);
    while (cinfo.output_scanline < cinfo.output_height) {
        u8* buffer_array[1];
        buffer_array[0] = bgr_buffer + (cinfo.output_scanline) * row_bytes;
        jpeg_read_scanlines(&cinfo, buffer_array, 1);
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    result->depth = 3;
    result->width = width;
    result->height = height;
    result->data = bgr_buffer;
	int i = 0;
	while (i < (width*height*3)){
		u8 tmp = result->data[i];
		result->data[i] = result->data[i+2];
		result->data[i+2] = tmp;
		i=i+3;
	}
	free(in);
    return result;
}

//revised --> Rinnegatamante :: Lua-Player-Plus :: Graphics.cpp :: void saveJpg(char *filename, u32 *pixels, u32 width, u32 height);
unsigned SaveJPG(const char* filename, imagebuff* image){
	FILE *outfile = fopen(filename, "wb");
	struct jpeg_error_mgr jerr;
	struct jpeg_compress_struct cinfo;
	JSAMPROW row_pointer[1];
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = image->width;
	cinfo.image_height = image->height;
	cinfo.input_components = image->depth;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	cinfo.num_components = 3;
	cinfo.dct_method = JDCT_FLOAT;
	jpeg_set_quality(&cinfo, 100, TRUE);
	jpeg_start_compress(&cinfo, TRUE);
	while( cinfo.next_scanline < cinfo.image_height ){
		row_pointer[0] = (unsigned char*)&image->data[ (cinfo.next_scanline * cinfo.image_width * cinfo.input_components) / 4];
		jpeg_write_scanlines( &cinfo, row_pointer, 1 );
	}
	jpeg_finish_compress( &cinfo );
	jpeg_destroy_compress( &cinfo );
	fclose(outfile);
}
