#include <3ds.h>
#include "image.h"

#define BI_RGB 0

typedef struct tagBITMAPFILEHEADER 
{
   u16   bfType;        // must be 'BM' 
   u32   bfSize;        // size of the whole .bmp file
   u16   bfReserved1;   // must be 0
   u16   bfReserved2;   // must be 0
   u32   bfOffBits;     
} BITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER
{
   u32   biSize;            // size of the structure
   s32   biWidth;           // image width
   s32   biHeight;          // image height //abs(biHeight) bottom .. top
   u16   biPlanes;          // bitplanes
   u16   biBitCount;        // resolution 
   u32   biCompression;     // compression
   u32   biSizeImage;       // size of the image
   s32   biXPelsPerMeter;   // pixels per meter X
   s32   biYPelsPerMeter;   // pixels per meter Y
   u32   biClrUsed;         // colors used
   u32   biClrImportant;    // important colors
} BITMAPINFOHEADER;

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
//& portions used --> http://tipsandtricks.runicsoft.com/Cpp/BitmapTutorial.html#chapter4 :: ConvertBMPToRGBBuffer
imagebuff * loadBMP(const char* path)
{
	int x, y;
	s32 *h, *w;

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
	FSFILE_Read(file, &bytesRead, 0x12, &(w), 4); //biWidth
	FSFILE_Read(file, &bytesRead, 0x16, &(h), 4); //biHeight
	FSFILE_Read(file, &bytesRead, 0x1C, &(result->depth), 2); //biBitCount
//	FSFILE_Read(file, &bytesRead, 0x22, &(biSizeImage), 4); 

	result->width = w;
	result->height = abs(h); //we should check non negitive & bufpos = (y - 1) * psw + x;
	
        // Calculating the size of a bitmap found --> https://msdn.microsoft.com/en-us/library/ms969901.aspx
	//biSizeImage = ((((biWidth * biBitCount) + 31) & ~31) >> 3) * biHeight:

	result->data = (u8*)malloc(result->height * result->width * 3);
	u8* tempbuf = (u8*)malloc(size-0x36);
	FSFILE_Read(file, &bytesRead, 0x36, tempbuf, size-0x36);

	FSFILE_Close(file);
	svcCloseHandle(file);

	int padding = 0;
	int scanlinebytes = result->width * 3;
	while ( ( scanlinebytes + padding ) % 4 != 0 )     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// swap the R and B bytes and the scanlines
	long bufpos = 0;   
	long newpos = 0;
	for (y = 0; y < result->height; y++ )
		for (x = 0; x < 3 * result->width; x+=3 )
		{
			newpos = y * 3 * result->width + x;     
			bufpos = (result->height - y - 1) * psw + x;

			result->data[newpos] = tempbuf[bufpos + 2];       
			result->data[newpos + 1] = tempbuf[bufpos+1]; 
			result->data[newpos + 2] = tempbuf[bufpos];     
		}
//         result->data = (u8*)ConvertBMPToRGBBuffer(tempbuf, result->width, result->height);
	free(tempbuf);
	return result;
}
