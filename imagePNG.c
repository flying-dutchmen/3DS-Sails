#include <3ds.h>
#include "image.h"
#define LODEPNG_COMPILE_PNG
#include "lodepng.h"

//revised --> Rinnegatamante :: Lua-Player-Plus :: Graphics.cpp :: Bitmap* decodePNGfile(const char* filename)
imagebuff* loadPNG(const char* filename)
{
	Handle fileHandle;
	imagebuff* result;
	u64 size;
	u32 bytesRead;
	unsigned char* out;
	unsigned char* in;
	unsigned int w, h;
	
	FS_path filePath = FS_makePath(PATH_CHAR, filename);
	FS_archive archive = (FS_archive) { ARCH_SDMC, (FS_path) { PATH_EMPTY, 1, (u8*)"" }};
	FSUSER_OpenFileDirectly(NULL, &fileHandle, archive, filePath, FS_OPEN_READ, FS_ATTRIBUTE_NONE);
	
	FSFILE_GetSize(fileHandle, &size);
	
	in = (unsigned char*)malloc(size);
	
	if(!in) {
		FSFILE_Close(fileHandle);
		svcCloseHandle(fileHandle);
		return 0;
	}
	
	FSFILE_Read(fileHandle, &bytesRead, 0x00, in, size);
	FSFILE_Close(fileHandle);
	svcCloseHandle(fileHandle);
		
		if(lodepng_decode32(&out, &w, &h, in, size) != 0) {
			free(in);
			return 0;
		}
	
	free(in);
	
	result = (imagebuff*)malloc(sizeof(imagebuff));
	if(!result) {
		free(out);
	}
	
	result->data = out;
	result->width = w;
	result->height = h;
	result->bitdepth = 32;
	return result;
}

unsigned SavePNG(const char* filename, imagebuff* image)
{
if (image->bitdepth = 32) 
  return (lodepng_encode32_file(filename, (unsigned char)image->data, image->width, image->height));
 else
  return (lodepng_encode24_file(filename, (unsigned char)image->data, image->width, image->height));
}
