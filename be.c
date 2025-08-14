#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct {
 Display*             display;
 XSetWindowAttributes attrs;
 Window               window;
 XSizeHints           size;
 XWMHints             hints;
 XTextProperty        windowName;
 XEvent               event;
 unsigned long        valuemask;
 int                  screenNum;
 GC                   gc; /* graphics context */
} State;

void terminate(State* s) {
 XUnmapWindow(s->display, s->window);
 XDestroyWindow(s->display, s->window);
 XCloseDisplay(s->display);
 free(s); 
}

State* initiate() {
 char* name = "be";

 State* s = malloc(sizeof(State));
 memset(s, 0, sizeof(State));
 s->display = XOpenDisplay(0);
 Window rootWin = XDefaultRootWindow(s->display);

 s->screenNum = DefaultScreen(s->display);
 s->gc = XDefaultGC(s->display, s->screenNum);

 int wX = 0;
 int wY = 0;
 int wWidth = 800;
 int wHeight = 600;
 int borderWidth = 0;
 int wDepth = CopyFromParent;
 int wClass = CopyFromParent;
 
 Visual *wVisual = CopyFromParent;

 int attrValMask = CWBackPixel | CWEventMask;
 XSetWindowAttributes wAttrs = {}; 

 wAttrs.background_pixel = 0xffffffff;
 wAttrs.event_mask = StructureNotifyMask | KeyPressMask 
                     | KeyReleaseMask | ExposureMask;

 
 s->window = XCreateWindow(s->display, rootWin, wX, wY, wWidth,
                     wHeight, borderWidth, wDepth, wClass,
                     wVisual, attrValMask, &wAttrs);

 
 XMapWindow(s->display, s->window);
 XStoreName(s->display, s->window, name);
 XFlush(s->display);
 
 return s;
}

int main(void) {
 State* s = initiate();
 
 KeySym sym;

 int shift = 0;
 Bool shouldClose = False;
 char* str = NULL;
 unsigned int size = 0;
 
 while(!shouldClose) {
  XNextEvent(s->display, &s->event);

  switch (s->event.type) {
   case KeyPress:
    sym = XLookupKeysym(&s->event.xkey, shift);
    printf("%c\n", (char)sym);

    free(str);
    size = 2 * sizeof(char);
    str = malloc(size);
    sprintf(str, "%c", (char)sym);
    
    if(sym == XK_Q || sym == XK_q) shouldClose = True;
	
    break;
   case KeyRelease:
    break;
  }
  
  XClearWindow(s->display, s->window);
  
  XDrawString(s->display, s->window, s->gc, 1, 10, str, size - 1);
 }
 
 free(str); 
 terminate(s); 
 return 0;
}
