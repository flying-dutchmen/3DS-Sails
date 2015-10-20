#include <3ds.h>
#include "image.h"

//revised --> AlbertoSONIC :: 3DS_Paint :: bool saveDrawing(char* path)
bool saveBMP(const char* path, imagebuff * image)
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

	u32 bitmapsize = image->width * image->height * 3;
	u8* tempbuf = (u8*)malloc(0x36 + bitmapsize);
	memset(tempbuf, 0, 0x36 + bitmapsize);

	FSFILE_SetSize(file, (u16)(0x36 + bitmapsize));

	*(u16*)&tempbuf[0x0] = 0x4D42;
	*(u32*)&tempbuf[0x2] = 0x36 + bitmapsize;
	*(u32*)&tempbuf[0xA] = 0x36;
	*(u32*)&tempbuf[0xE] = 0x28;
	*(u32*)&tempbuf[0x12] = image->width; // width
	*(u32*)&tempbuf[0x16] = image->height; // height
	*(u32*)&tempbuf[0x1A] = 0x00180001;
	*(u32*)&tempbuf[0x22] = bitmapsize;

	for (y = 0; y < image->height; y++)
	{
		for (x = 0; x < image->width; x++)
		{
			int si = (((image->height-1) - y) + (x * image->height)) * image->depth;
			int di = 0x36 + (x + ((image->height-1) - y) * image->width) * image->depth;

			tempbuf[di++] = image->data[si++];
			tempbuf[di++] = image->data[si++];
			tempbuf[di++] = image->data[si++];
		}
	}

	FSFILE_Write(file, &byteswritten, 0, (u32*)tempbuf, 0x36 + bitmapsize, 0x10001);

	FSFILE_Close(file);
	free(tempbuf);
	return true;
}

//found --> http://tipsandtricks.runicsoft.com/Cpp/BitmapTutorial.html#chapter4
u8* ConvertBMPToRGBBuffer (u8* Buffer, int width, int height )
{
 int x, y;
	// first make sure the parameters are valid
	if ( ( NULL == Buffer ) || ( width == 0 ) || ( height == 0 ) )
		return NULL;

	// find the number of padding bytes
		
	int padding = 0;
	int scanlinebytes = width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
        u8* newbuf = (u8*)malloc(width*height*3);
//	u8* newbuf = new BYTE[width*height*3];
	
	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;   
	long newpos = 0;
	for (y = 0; y < height; y++ )
		for (x = 0; x < 3 * width; x+=3 )
		{
			newpos = y * 3 * width + x;     
			bufpos = ( height - y - 1 ) * psw + x;

			newbuf[newpos] = Buffer[bufpos + 2];       
			newbuf[newpos + 1] = Buffer[bufpos+1]; 
			newbuf[newpos + 2] = Buffer[bufpos];     
		}

	return newbuf;
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
	FSFILE_Read(file, &bytesRead, 0x12, &(result->width), 4); //biWidth
	FSFILE_Read(file, &bytesRead, 0x16, &(result->height), 4); //biHeight
	FSFILE_Read(file, &bytesRead, 0x1C, &(result->depth), 2); //biBitCount


// Calculating the size of a bitmap found --> https://msdn.microsoft.com/en-us/library/ms969901.aspx
	//biSizeImage = ((((biWidth * biBitCount) + 31) & ~31) >> 3) * biHeight:

//	result->data = (u8*)malloc(size-0x36);
	u8* tempbuf = (u8*)malloc(size-0x36);
	FSFILE_Read(file, &bytesRead, 0x36, tempbuf, size-0x36);

	FSFILE_Close(file);
	svcCloseHandle(file);

        result->data = (u8*)ConvertBMPToRGBBuffer(tempbuf, result->width, result->height);
	free(tempbuf);
	return result;
}
