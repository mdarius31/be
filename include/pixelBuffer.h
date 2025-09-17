#ifndef BE_PIXEL_BUFFER_H
#define BE_PIXEL_BUFFER_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "font.h"

typedef struct { 
 unsigned char r;
 unsigned char g;
 unsigned char b;
 unsigned char a;
} Pixel;

Pixel pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
Pixel solidPixel(unsigned char r, unsigned char g, unsigned char b);
Pixel blendPixels(Pixel base, Pixel add);


typedef struct {
 Pixel* data;
 int width;
 int height;
} PixelBuffer;

typedef struct {
 int w, h;
} Rect;

void drawPixelPB(PixelBuffer* buf, int x, int y, Pixel color);
PixelBuffer* freePB(PixelBuffer* buf);
PixelBuffer* newPB(int width, int height);
void clearPB(PixelBuffer* buf, Pixel color);
void drawRectPB(PixelBuffer* buf, Pixel color,int rx,int ry, Rect r);
void drawCharPB(PixelBuffer* buf, Pixel fg, Pixel bg, int x, int y, unsigned char code, AsciiFont* font, float scale);
bool resizePB(PixelBuffer* buf, int width, int height);

/* BE_PIXEL_BUFFER_H */
#endif
