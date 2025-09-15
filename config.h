#define TARGET x11
#define DEBUG

#ifndef BUILD_SCRIPT

 #if TARGET == x11
  #include "x11.c"
 #elif TARGET == gtk
  #include "gtk.c"
 #endif

#endif 
