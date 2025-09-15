#include <stdio.h>
#include <string.h>

#include "graphic.c"
typedef struct {
 unsigned int updateNum;
 unsigned int updateNumAim;
 
 Pixel fg, bg;
 PixelBuffer* buf;

 Graphic** entities;
 unsigned int entitiesNum;
 AsciiFont* font;
 int zoom;
} Content;

void clearContent(Content* content) {
 clearPB(content->buf, content->bg);
}

Content* newContent(Pixel fg, Pixel bg, int width, int height, AsciiFont* font) {
 Content* res = malloc(sizeof(Content));
 res->fg = fg;
 res->bg = bg;
 res->buf = newPB(width, height);
 res->entities = NULL;
 res->entitiesNum = 0;
 res->updateNum = 0;
 res->updateNumAim = 0;
 res->font = font;
 res->zoom = 1;
 
 clearContent(res);

 return res; 
}


Content* freeContent(Content* content) {
 content->buf = freePB(content->buf);

 unsigned int i;
 for(i = 0; i < content->entitiesNum; i++) {
  free(content->entities[i]);
 }
 free(content->entities);

 free(content);
 content = NULL;
 
 return content;
}


int compareEntitiesByZIndex(const void* a, const void* b) {
 Graphic** GraphicPtrA = (Graphic**)a;
 Graphic** GraphicPtrB = (Graphic**)b;

 Graphic* GraphicA = *GraphicPtrA;
 Graphic* GraphicB = *GraphicPtrB;
 
 return GraphicA->zIndex - GraphicB->zIndex;
}

void addGraphic(Content* content, Graphic* ent) {
 content->entitiesNum += 1;
 content->entities = realloc(content->entities, sizeof(Graphic) * content->entitiesNum);
 content->entities[content->entitiesNum - 1] = ent;
 
 content->updateNum += 1;
}


void drawGraphic(Content* content, Graphic* ent) {
 if(ent->type == CHAR) {
   drawCharPB(content->buf, ent->fg, ent->bg, ent->x, ent->y, ent->val.code, content->font, content->zoom);
 }
}

bool renderContent(Content* content) {
 if(content->updateNum == content->updateNumAim) return false;
 
 qsort(content->entities, content->entitiesNum, sizeof(Graphic*), compareEntitiesByZIndex);
 
 unsigned int i;
 for(i = 0; i < content->entitiesNum; i++) {
  drawGraphic(content, content->entities[i]);
 }

 content->updateNum = content->updateNumAim;
 
 return true;
}


Content* resizeContent(Content* content, int width, int height) {
 if(resizePB(content->buf, width, height)) {
  content->updateNum += 1;
 }
 
 return content;
}

Content* defaultContent(int width, int height) {
 Pixel bg = (Pixel){38,38,38};
 Pixel fg = (Pixel){133,133,133};
 Content* content = newContent(fg, bg, width, height, &defaultFont);
 content->zoom = 10; 
 
 // testing colors
 addGraphic(content, newCharGraphic(CHAR, 10, 10, 0, fg, bg, '#'));
 addGraphic(content, newCharGraphic(CHAR, 100, 10, 5, (Pixel){255, 0, 0}, bg, '%'));
 addGraphic(content, newCharGraphic(CHAR, 200, 10, 0, (Pixel){0, 255, 0} , (Pixel){0, 0, 0}, '$'));

 // testing characters
 char* test = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;':\\\",./<>?`~";
 size_t i;
 for(i = 0; i < strlen(test); i++) addGraphic(content, newCharGraphic(CHAR, 10 * (i + 1), 30, 0,  fg, bg, test[i]));
 return content;
}
