#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
 if(s == NULL) {
  return;
 }
 XUnmapWindow(s->display, s->window);
 XDestroyWindow(s->display, s->window);
 XCloseDisplay(s->display);
 free(s->windowName.value);
 free(s);
 s = NULL;
}

State* initiate() {
 char* name = "be";
 State* s = malloc(sizeof(State));
 
 unsigned int xPos, yPos, width, height;
 xPos = 0;
 yPos = 0;
 width = 500;
 height = 500;

 s->display = XOpenDisplay(NULL);
 if(s->display == NULL) {
  fprintf(stderr, "Terminating early due to failure of opening a connection to X11 server\n");
  terminate(s);
  return NULL;
 }
 
 s->screenNum = DefaultScreen(s->display);
 s->gc = DefaultGC(s->display, s->screenNum);
 
 s->attrs.background_pixel = XWhitePixel(s->display, s->screenNum);
 s->attrs.border_pixel = XBlackPixel(s->display, s->screenNum);
 s->attrs.event_mask = ButtonPressMask;
 s->valuemask = CWBackPixel | CWBorderPixel | CWEventMask;

 Window rootWindow = RootWindow(s->display, s->screenNum);
 int depth = XDefaultDepth(s->display, s->screenNum);
 Visual* visual = XDefaultVisual(s->display, s->screenNum);
 
 s->window = XCreateWindow(s->display, rootWindow, xPos,
 yPos, width, height, 1, depth, InputOutput,
 visual, s->valuemask, &s->attrs);

 s->size.flags = USPosition | USSize;
 XSetWMNormalHints(s->display, s->window, &s->size);
 
 XStringListToTextProperty(&name, 1, &s->windowName);
 XSetWMName(s->display, s->window, &s->windowName);

 XSelectInput(s->display, s->window, ExposureMask | KeyPressMask);
 
 s->hints.initial_state = NormalState;
 s->hints.flags = StateHint;
 XSetWMHints(s->display, s->window, &s->hints);
 

 XMapWindow(s->display, s->window);
  
 return s;
}

int main(void) {
 State* s = initiate();
 char* msg = "Hello, World!";
 
 while(1) {
  XNextEvent(s->display, &s->event);
 
  if (s->event.type == Expose) {
   XFillRectangle(s->display, s->window, s->gc, 20, 20, 10, 10);
   XDrawString(s->display, s->window, s->gc, 50, 50, msg, strlen(msg));
  }
 
  if (s->event.type == KeyPress) break;
 }
 terminate(s);
 
 return 0;
}
