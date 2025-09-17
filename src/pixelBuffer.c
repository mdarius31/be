#include "../include/pixelBuffer.h"

Pixel pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
 return (Pixel){r, g ,b ,a};
}

Pixel solidPixel(unsigned char r, unsigned char g, unsigned char b) {
 return (Pixel){r, g, b, 255};
}

Pixel blendPixels(Pixel base, Pixel add) {
 unsigned char whole = 255;
 
 if(add.a == whole)
   return add;
 
 Pixel res = base;
 
 float addPer = ((float)add.a / whole) * 100;
 float basePer = 100 - addPer;

 float baseDecimal = (basePer / 100);
 float addDecimal = (addPer / 100);
 
 res.r = (baseDecimal * base.r) + (addDecimal * add.r);
 res.g = (baseDecimal * base.g) + (addDecimal * add.g);
 res.b = (baseDecimal * base.b) + (addDecimal * add.b);
 res.a = (baseDecimal * base.a) + (addDecimal * add.a);
        
 
 return res;
}

void drawPixelPB(PixelBuffer* buf, int x, int y, Pixel color) {
 int i = (y * buf->width) + x;

 bool isWithinHBounds = x >= 0 && x < buf->width;
 bool isWithinVBounds = y >= 0 && y < buf->height;
 
 if (isWithinHBounds && isWithinVBounds) {
  buf->data[i] = blendPixels(buf->data[i], color); 
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
 
 int len = width * height;
 int size = sizeof(Pixel) * len;
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

void drawCharPB(PixelBuffer* buf, Pixel fg, Pixel bg, int x, int y, unsigned char code, AsciiFont* font, float scale) {
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

 for(chx = 0; chx < font->width; chx++) {
  for(chy = 0; chy < font->height; chy++) {
   i = (chy*font->width) + chx;
   fx = x+chx;
   fy = y+chy;
   
   if(art[i] == '.') color = bg;
   if(art[i] == '#') color = fg;

   int sx, sy;
   for (sx = 0; sx < scale; sx++) {
    for (sy = 0; sy < scale; sy++) {
     fx = x + (chx * scale) + sx;
     fy = y + (chy * scale) + sy;
     drawPixelPB(buf, fx, fy, color);
    }
   }
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

