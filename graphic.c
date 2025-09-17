#include "pixelBuffer.c"

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

Graphic* newGraphic(GraphicType type, int x, int y, int zIndex, Pixel fg, Pixel bg) {
 Graphic* res = malloc(sizeof(Graphic));

 res->type = type;

 res->x = x;
 res->y = y;
 res->zIndex = zIndex;

 res->fg = fg;
 res->bg = bg;

 res->parent = NULL;
 res->childrenLen = 0;
 res->children = NULL;
 
 return res;
}

Graphic* newCharGraphic(int x, int y, int zIndex, Pixel fg, Pixel bg, unsigned char code) {
 Graphic* res = newGraphic(CHAR, x, y, zIndex, fg, bg);

 res->val.code = code;

 return res;
}
