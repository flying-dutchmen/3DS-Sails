#include <3ds.h>

typedef struct
{
	u16 width, height;
	u8* data;
	unsigned bitdepth;
	bool used;
}imagebuff;
