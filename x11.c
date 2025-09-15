#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "content.c"
#define INIT_WIDTH 800
#define INIT_HEIGHT 600

typedef struct {
 Display*             display;
 XSetWindowAttributes attrs;
 Window               window;
 XSizeHints           size;
 XWMHints             hints;
 XTextProperty        windowName;
 XEvent               event;
 int                  depth;
 unsigned long        valuemask;
 int                  screenNum;
 GC                   gc; /* graphics context */
 Visual*              visual;
} WinState;

void terminateWin(WinState* s) {
 XUnmapWindow(s->display, s->window);
 XDestroyWindow(s->display, s->window);
 XCloseDisplay(s->display);
 free(s); 
}

WinState* initiateWin() {
 char* name = "be";

 WinState* s = malloc(sizeof(WinState));
 memset(s, 0, sizeof(WinState));
 s->display = XOpenDisplay(0);
 Window rootWin = XDefaultRootWindow(s->display);

 s->screenNum = DefaultScreen(s->display);
 s->gc = XDefaultGC(s->display, s->screenNum);
 s->depth = XDefaultDepth(s->display, s->screenNum);
 s->visual = CopyFromParent;
 
 int wX = 0;
 int wY = 0;
 int wWidth = INIT_WIDTH;
 int wHeight = INIT_HEIGHT;
 int borderWidth = 0;
 int wDepth = CopyFromParent;
 int wClass = CopyFromParent;
 
 

 int attrValMask = CWBackPixel | CWEventMask;
 XSetWindowAttributes wAttrs = {}; 

 wAttrs.background_pixel = 0xffffffff;
 wAttrs.event_mask = StructureNotifyMask | KeyPressMask 
                     | KeyReleaseMask | ExposureMask;

 
 s->window = XCreateWindow(s->display, rootWin, wX, wY, wWidth,
                     wHeight, borderWidth, wDepth, wClass,
                     s->visual, attrValMask, &wAttrs);

 
 XMapWindow(s->display, s->window);
 XStoreName(s->display, s->window, name);
 XFlush(s->display);
 
 return s;
}


XImage* pixelBuffer2XImage(WinState* s, PixelBuffer* buf) {
 if (!s || !buf->data) {
  return NULL;
 }

 XImage* ximage = XCreateImage(
     s->display,
     s->visual,
     s->depth,
     ZPixmap,
     0,
     NULL,
     buf->width,
     buf->height,
     32,
     0
 );

 if (!ximage) {
  return NULL;
 }

 ximage->data = malloc(ximage->bytes_per_line * ximage->height);
 
 if (!ximage->data) {
  XDestroyImage(ximage);
  return NULL;
 }

 int x, y;
 
 for (x = 0; x < buf->width; ++x) {
  for (y = 0; y < buf->height; ++y) {
   const Pixel* pixel = &buf->data[y * buf->width + x];

   /* Xlib specific byte order */
   unsigned long x11PixelValue = (pixel->r << 16) | (pixel->g << 8) | pixel->b;
   
   XPutPixel(ximage, x, y, x11PixelValue);
  }
 }

 return ximage;
}

XImage* content2XImage(WinState* s, Content* content) {
 return pixelBuffer2XImage(s, content->buf);
}

void runBe() {
 
 Content* content = defaultContent(INIT_WIDTH, INIT_HEIGHT);

  renderContent(content);

 
 WinState* s = initiateWin();

 XImage *img = content2XImage(s, content);
  
 KeySym sym;

 int shift = 0;
 Bool shouldClose = False;
 char* str = NULL;
 unsigned int size = 0;

 int cw = INIT_WIDTH;
 int ch = INIT_HEIGHT;
 
 
 while(!shouldClose) {
  XNextEvent(s->display, &s->event);

  switch (s->event.type) {
  
   case KeyPress:
   
    sym = XLookupKeysym(&s->event.xkey, shift);
    printf("%i ", s->event.xkey.keycode);
    printf("%c\n", (char)sym);

    if(str != NULL) free(str);
    size = 2 * sizeof(char);
    str = malloc(size);
    sprintf(str, "%c", (char)sym);
    
    if(sym == XK_Escape) shouldClose = True;
	
    break;

   case KeyRelease:
    break;

   case ConfigureNotify:
    XConfigureEvent xce = s->event.xconfigure;
    if (xce.width != cw || xce.height != ch) {
     cw = xce.width;
     ch = xce.height;
     
     content = resizeContent(content, cw, ch);
     
 
      
     XDestroyImage(img);
     img = content2XImage(s, content);    
    } 
    break;
  }


  /*  
  XClearWindow(s->display, s->window);
  XDrawString(s->display, s->window, s->gc, 1, 10, str, size-1);
  */

  clearContent(content);
  
  if(renderContent(content)) {
   XDestroyImage(img);
   img = content2XImage(s, content);    
  }
 
  XPutImage(s->display, s->window, s->gc, img, 0, 0, 0, 0, img->width, img->height);

 } 

 free(str); 
 terminateWin(s);
 content = freeContent(content);
 XDestroyImage(img);
}
