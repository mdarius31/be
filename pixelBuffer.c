#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "font.c"

typedef struct { 
 unsigned char r;
 unsigned char g;
 unsigned char b;
} Pixel;

typedef struct {
 Pixel* data;
 int width;
 int height;
} PixelBuffer;

void drawPixelPB(PixelBuffer* buf, int x, int y, Pixel color) {
 int i = (y * buf->width) + x;
 
 if (x >= 0 && x < buf->width && y >= 0 && y < buf->height) {
   buf->data[i] = color;
 } 
}

PixelBuffer* freePB(PixelBuffer* buf) {
 free(buf->data);
 buf->data = NULL;
 free(buf);
 buf = NULL;
 
 return buf;
}

PixelBuffer* newPB(int width, int height) {
 PixelBuffer* res = malloc(sizeof(PixelBuffer));
 
 int size = sizeof(Pixel) * width * height;
 res->data = malloc(size);
 res->width = width;
 res->height = height;
 
 return res;
}

void clearPB(PixelBuffer* buf, Pixel color) {
 int x, y;

 for(x = 0; x < buf->width; x++) {
  for(y = 0; y < buf->height; y++) {
   int i = (y * buf->width) + x;
   buf->data[i] = color;
  }
 }
}

typedef struct {
 int w, h;
} Rect;

void drawRectPB(PixelBuffer* buf, Pixel color,int rx,int ry, Rect r) {
 int x, y;
 
 for(x = 0; x < r.w; x++) {
  for(y = 0; y < r.h; y++) {
   int cx = rx + x;
   int cy = ry + y;
   
   drawPixelPB(buf, cx, cy, color);
  }
 }
}

void drawCharPB(PixelBuffer* buf, Pixel fg, Pixel bg, int x, int y, unsigned char code, AsciiFont* font, int zoom) {
 char* art = font->notFound;
 
 int i;
 for(i = 0; i < font->len; i++) {
  if(font->ascii[i].code == code) {
   art = font->ascii[i].art;
   break;
  }
 }

 int chx, chy, fx, fy;
 Pixel color;
 
 (void)(zoom);
 for(chx = 0; chx < font->width; chx++) {
  for(chy = 0; chy < font->height; chy++) {
   i = (chy*font->width) + chx;
   fx = x+chx;
   fy = y+chy;
   
   if(art[i] == '.') color = bg;
   if(art[i] == '#') color = fg;
   
   drawPixelPB(buf, fx, fy, color);

  }
 }

}

bool resizePB(PixelBuffer* buf, int width, int height) {
 if(buf->width == width && buf->height == height) return false;
 
 buf->width = width;
 buf->height = height;
 
 unsigned int size = sizeof(Pixel) * width * height;
 buf->data = realloc(buf->data, size);
 
 return true;
}

