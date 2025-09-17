#include "../include/content.h"

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
 res->scale = 1;
 
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
   drawCharPB(content->buf, ent->fg, ent->bg, ent->x, ent->y, ent->val.code, content->font, content->scale);
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

Content* testContent(int width, int height) {
 Pixel bg = solidPixel(38,38,38);
 Pixel fg = solidPixel(133,133,133);
 Content* content = newContent(fg, bg, width, height, &defaultFont);
 content->scale = 2.1;
 
 /* testing colors */
 addGraphic(content, newCharGraphic(10, 300, 0, fg, bg, '#'));
 addGraphic(content, newCharGraphic(100, 300, 5, solidPixel(255, 0, 0), bg, '%'));
 addGraphic(content, newCharGraphic(200, 300, 0, pixel(0, 255, 0, 100), pixel(0, 0, 0, 0), '$'));
 addGraphic(content, newCharGraphic(300, 300, 0, fg, bg, '$'));

 /* testing characters */
 char* test = "abcdefghijklmnopqrstuvwxyz"
              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
              "0123456789!@#$%^&*()_+-=[]"
              "{}|;':\\\",./<>?`~";
              
 size_t i = 0;
 float x = 0;
 float y = 0;
 int gap = 2;
 float xPosDif = (content->font->width + gap) * content->scale;
 float yPosDif = (content->font->height + gap) * content->scale;
 for(i = 0; i < strlen(test); i++) {

  if(i > 0) x += xPosDif;
  if((i % 26) == 0) {
   y += yPosDif;
   x = xPosDif;
  }
  
  addGraphic(content, newCharGraphic(x, y, 10,  fg, bg, test[i]));
 }
 
 addGraphic(content, newCharGraphic(10, 500, 0, fg , bg, 255));
 
 return content;
}

Content* defaultContent(int width, int height) {
 Pixel bg = solidPixel(38,38,38);
 Pixel fg = solidPixel(133,133,133);

 Content* content = newContent(fg, bg, width, height, &defaultFont);
 content->scale = 2;

 return content;
}
