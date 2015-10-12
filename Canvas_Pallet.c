//forked --> https://github.com/mtheall/screenshot_png
//bytes per pixel, for handling the Framebuffers pallet of colours

#include <3ds.h>

//found @ https://github.com/Lectem/3Damnesic 
#define ABGR8(r, g, b, a) ((((r)&0xFF)<<24) | (((g)&0xFF)<<16) | (((b)&0xFF)<<8) | (((a)&0xFF)<<0))
//& found //gs.h --> crtulib nintendo 3ds
//#define RGBA8(r,g,b,a) ((((r)&0xFF)<<24) | (((g)&0xFF)<<16) | (((b)&0xFF)<<8) | (((a)&0xFF)<<0))

// --> screenshot_png.c :: bytes_per_pixel
u32 bytes_per_pixel(GSP_FramebufferFormats format)
{
  switch(format)
  {
    case GSP_RGBA8_OES:
      return 4;
    case GSP_BGR8_OES:
      return 3;
    case GSP_RGB565_OES:
    case GSP_RGB5_A1_OES:
    case GSP_RGBA4_OES:
      return 2;
  }

  return 3;
}

// --> main.c :: rgb_to_pixel
void rgb_to_pixel(u8 *dst, const u8 *src, GSP_FramebufferFormats format)
{
  u16 half;

  switch(format)
  {
    case GSP_RGBA8_OES:
      dst[0] = 0xFF;
      dst[1] = src[2];
      dst[2] = src[1];
      dst[3] = src[0];
      break;

    case GSP_BGR8_OES:
      dst[0] = src[2];
      dst[1] = src[1];
      dst[2] = src[0];
      break;

    case GSP_RGB565_OES:
      half  = (src[0] & 0xF8) << 8;
      half |= (src[1] & 0xFC) << 3;
      half |= (src[2] & 0xF8) >> 3;
      memcpy(dst, &half, sizeof(half));
      break;

    case GSP_RGB5_A1_OES:
      half  = (src[0] & 0xF8) << 8;
      half |= (src[1] & 0xF8) << 3;
      half |= (src[2] & 0xF8) >> 2;
      half |= 1;
      memcpy(dst, &half, sizeof(half));
      break;

    case GSP_RGBA4_OES:
      half  = (src[0] & 0xF0) << 8;
      half |= (src[1] & 0xF0) << 4;
      half |= (src[2] & 0xF0) << 0;
      half |= 0x0F;
      memcpy(dst, &half, sizeof(half));
      break;
  }
}

// --> screenshot_png.c :: pixel_to_rgba
void pixel_to_rgba(u8 *dst, const u8 *src, GSP_FramebufferFormats format)
{
  u16 half;

  switch(format)
  {
    case GSP_RGBA8_OES:
      dst[0] = src[3];
      dst[1] = src[2];
      dst[2] = src[1];
      dst[3] = src[0];
      break;

    case GSP_BGR8_OES:
      dst[0] = src[2];
      dst[1] = src[1];
      dst[2] = src[0];
      dst[3] = 0xFF;
      break;

    case GSP_RGB565_OES:
      memcpy(&half, src, sizeof(half));
      dst[0] = (half >> 8) & 0xF8;
      dst[1] = (half >> 3) & 0xFC;
      dst[2] = (half << 3) & 0xF8;
      dst[3] = 0xFF;
      break;

    case GSP_RGB5_A1_OES:
      memcpy(&half, src, sizeof(half));
      dst[0]  = (half >> 8) & 0xF8;
      dst[0] |= dst[0] >> 5;
      dst[1]  = (half >> 3) & 0xF8;
      dst[0] |= dst[1] >> 5;
      dst[2]  = (half << 2) & 0xF8;
      dst[0] |= dst[2] >> 5;
      dst[3]  = half & 1 ? 0xFF : 0x00;
      break;

    case GSP_RGBA4_OES:
      memcpy(&half, src, sizeof(half));
      dst[0]  = (half >> 8) & 0xF0;
      dst[0] |= dst[0] >> 4;
      dst[1]  = (half >> 4) & 0xF0;
      dst[1] |= dst[1] >> 4;
      dst[2]  = (half >> 0) & 0xF0;
      dst[2] |= dst[2] >> 4;
      dst[3]  = (half << 4) & 0xF0;
      dst[3] |= dst[3] >> 4;
      break;
  }
}
