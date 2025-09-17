#ifndef BE_CONTENT_H
#define BE_CONTENT_H

#include <stdio.h>
#include <string.h>

#include "graphic.h"

typedef struct {
 unsigned int updateNum;
 unsigned int updateNumAim;
 
 Pixel fg, bg;
 PixelBuffer* buf;

 Graphic** entities;
 unsigned int entitiesNum;
 AsciiFont* font;
 float scale;
} Content;

void clearContent(Content* content);
Content* newContent(Pixel fg, Pixel bg, int width, int height, AsciiFont* font);
Content* freeContent(Content* content);
int compareEntitiesByZIndex(const void* a, const void* b);
void addGraphic(Content* content, Graphic* ent);
void drawGraphic(Content* content, Graphic* ent);
bool renderContent(Content* content);
Content* resizeContent(Content* content, int width, int height);
Content* testContent(int width, int height);
Content* defaultContent(int width, int height);


/* BE_CONTENT_H */
#endif
