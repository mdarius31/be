#ifndef BE_GRAPHIC_H
#define BE_GRAPHIC_H

#include "pixelBuffer.h"

typedef enum {
 CHAR
} GraphicType;

typedef union {
 unsigned char code;
} GraphicValue;

typedef struct Graphic Graphic;
typedef struct Graphic {
 GraphicType type;
 GraphicValue val;

 int x, y;
 int zIndex;

 Pixel fg, bg;
 Graphic* parent;
 Graphic* children;
 unsigned int childrenLen;
} Graphic;

Graphic* newGraphic(GraphicType type, int x, int y, int zIndex, Pixel fg, Pixel bg);
Graphic* newCharGraphic(int x, int y, int zIndex, Pixel fg, Pixel bg, unsigned char code);

/* BE_GRAPHIC_H */
#endif
