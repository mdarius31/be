#define BE_TARGET x11
#define BE_DEBUG

#ifndef BE_BUILD_SCRIPT

 #if BE_TARGET == x11
  #include "../src/x11.c"
 #elif BE_TARGET == gtk
  #include "../src/gtk.c"
 #endif

#endif 
