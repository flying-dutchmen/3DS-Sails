#include <stdarg.h>
#include "Canvas_font.h"

//CanvasChar && CanvasString
//revised --> https://bitbucket.org/xerpi/eleven-arms/src/

extern const u8 msx_font[];

//int font_draw_char(int x, int y, u32 color, char c)
int CanvasChar(u8* screen,  char c, int x, int y, u32 colour)
{
	u8 *font = (u8*)(msx_font + (c - (u32)' ') * 8);
	int i, j;
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 8; ++j) {
//			if ((*font & (128 >> j))) draw_plot(x+j, y+i, color);
			if ((*font & (128 >> j))) SetCanvasPixel(screen, x+j, y+i, colour);
		}
		++font;
	}
	return x+8;
}

//int font_draw_string(int x, int y, u32 color, const char *string)
int CanvasString(u8* screen, const char *string, int x, int y, u32 colour)
{
	if (string == NULL) return x;
	int startx = x;
	const char *s = string;
	while (*s) {
		if (*s == '\n') {
			x = startx;
			y+=8;
		} else if (*s == ' ') {
			x+=8;
		} else if(*s == '\t') {
			x+=8*4;
		} else {
//			draw_fillrect(x, y, 8, 8, BLACK);
//			font_draw_char(x, y, color, *s);
			CanvasChar(screen, *s, x, y, colour);
			x+=8;
		}
		++s;
	}
	return x;
}

// punks out 
void Canvas_printf(u8* screen, int x, int y, const char* str,...)
{
    char* formated[256];
    va_list ap;
    va_start(ap, str);
    snprintf(formated, 256, str, ap);
    //sprintf(formated, str, ap);
    va_end(ap);
    CanvasString(screen, formated, x, y, WHITE);
}
