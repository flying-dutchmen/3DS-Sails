/*  fist draft too be tested */

#include <3ds.h>
#include "image.h"

//revised --> AlbertoSONIC :: 3DS_Paint :: bool saveDrawing(char* path)
bool saveBMP(u8* screen, const char* path, int h, int w)
{
	int x, y;

	Handle file;
	FS_path filePath;
        FS_archive sdmcArchive;
	filePath.type = PATH_CHAR;
	filePath.size = strlen(path) + 1;
	filePath.data = (u8*)path;

	Result res = FSUSER_OpenFile(NULL, &file, sdmcArchive, filePath, FS_OPEN_CREATE | FS_OPEN_WRITE, FS_ATTRIBUTE_NONE);
	if (res)
		return false;

	u32 byteswritten;

	u32 bitmapsize = w * h * 3;
	u8* tempbuf = (u8*)malloc(0x36 + bitmapsize);
	memset(tempbuf, 0, 0x36 + bitmapsize);

	FSFILE_SetSize(file, (u16)(0x36 + bitmapsize));

	*(u16*)&tempbuf[0x0] = 0x4D42;
	*(u32*)&tempbuf[0x2] = 0x36 + bitmapsize;
	*(u32*)&tempbuf[0xA] = 0x36;
	*(u32*)&tempbuf[0xE] = 0x28;
	*(u32*)&tempbuf[0x12] = h; // width
	*(u32*)&tempbuf[0x16] = w; // height
	*(u32*)&tempbuf[0x1A] = 0x00180001;
	*(u32*)&tempbuf[0x22] = bitmapsize;

	for (y = 0; y < h; y++)
	{
		for (x = 0; x < w; x++)
		{
			int si = (((h-1) - y) + (x * h)) * 3;
			int di = 0x36 + (x + (((h-1) - y) * w)) * 3;

			tempbuf[di++] = screen[si++];
			tempbuf[di++] = screen[si++];
			tempbuf[di++] = screen[si++];
		}
	}

	FSFILE_Write(file, &byteswritten, 0, (u32*)tempbuf, 0x36 + bitmapsize, 0x10001);

	FSFILE_Close(file);
	free(tempbuf);
	return true;
}

//revised --> Rinnegatamante :: Lua-Player-Plus :: Graphics.cpp :: Bitmap* decodeBMPfile(const char* fname)
imagebuff * loadBMP(const char* path)
{
	int x, y;

	Handle file;
	u32 bytesRead;
	u64 size;
	FS_path filePath = FS_makePath(PATH_CHAR, path);
	FS_archive archive = (FS_archive) { ARCH_SDMC, (FS_path) { PATH_EMPTY, 1, (u8*)"" }};
	FSUSER_OpenFileDirectly(NULL, &file, archive, filePath, FS_OPEN_READ, FS_ATTRIBUTE_NONE);

	FSFILE_GetSize(file, &size);
	imagebuff* result = (imagebuff*)malloc(sizeof(imagebuff));
	
	if(!result) {
		FSFILE_Close(file);
		svcCloseHandle(file);
		return 0;
	}
	
	result->data = (u8*)malloc(size-0x36);
	FSFILE_Read(file, &bytesRead, 0x36, result->data, size-0x36);
	FSFILE_Read(file, &bytesRead, 0x12, &(result->width), 4);
	FSFILE_Read(file, &bytesRead, 0x16, &(result->height), 4);
	FSFILE_Read(file, &bytesRead, 0x1C, &(result->bitdepth), 2);

	svcCloseHandle(file);
	FSFILE_Close(file);
	return result;
}
