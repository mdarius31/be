#include <stdio.h>
#include <stdlib.h>
#include "helper.c"

#define BUILD_SCRIPT
#include "config.h"

int main(void) {
 #ifdef DEBUG
  char* cmd = "cc be.c -g -Wextra -Wall -Werror --std=c89 -ansi -o be -lX11";
 #else
 
  #if TARGET == x11
   char* cmd = "cc be.c -o be -lX11";
  #elif TARGET == gtk
   char* cmd = "cc be.c -o be `pkg-config --cflags --libs gtk+-3.0`";   
  #endif

 #endif
 
 printf("%s\n", cmd);
 system(cmd);
 
 return 0;
}
