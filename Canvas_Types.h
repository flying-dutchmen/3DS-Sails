typedef enum
{
  generic = 0,
	linear = 1,
	vram = 2 
}RamType;

typedef struct point
{
   int   x;
   int   y;
} point;

typedef struct rect
{
   int   x;
   int   y;
   int   h;
   int   w;
} rect;
