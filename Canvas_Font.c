#include "Canvas_font.h"
#include "ascii64.h"

void CanvasChar(u8* screen, char letter,int x,int y, u32 colour)
{
	int i, k;
	unsigned char mask;
	unsigned char l;

	for (i = 0; i < 8; i++){
		mask = 0b10000000;
		l = asciiData[(int)letter][i];
		for (k = 0; k < 8; k++){
			if ((mask >> k) & l){
//				drawPixel(k+x,i+y,r,g,b,screen);
				SetCanvasPixel(screen, k+x,i+y,colour);
			}     
		}
	}
}

void CanvasString(u8* screen, char* word, int x, int y, u32 colour)
{
	int tmp_x =x;
	int i;
	int line = 0;
	int width = MaxWidth(screen);

//	switch(screenPos){
//	case GFX_BOTTOM:
//		=BOTTOM_WIDTH;
//		break;
//	default:
//		width=TOP_WIDTH;
//		break;
//	}

	for (i = 0; i < (signed)strlen(word); i++){

		if (tmp_x+8 > width) {
			line++;
			tmp_x = x;
		}
//		drawChar(word[i],tmp_x,y+(line*8),r,g,b, screen);
		CanvasChar(screen, word[i],tmp_x,y+(line*8),colour);

		tmp_x = tmp_x+8;
	}
}
