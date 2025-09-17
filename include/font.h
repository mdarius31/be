#ifndef BE_FONT_H
#define BE_FONT_H

typedef struct {
 unsigned char code;
 char* art;
} Ascii;

typedef struct {
 Ascii ascii[256];
 int len;
 int width;
 int height;
 char* notFound;
} AsciiFont;

extern AsciiFont defaultFont;

/* BE_FONT_H */
#endif
