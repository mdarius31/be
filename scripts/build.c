#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/helper.c"

#define BE_BUILD_SCRIPT
#include "../include/config.h"

#define olderC(filename) older("../build/" #filename ".o", "../src/" #filename ".c") || older("../build/" #filename ".o", "../include/" #filename ".h")

void build(char* cmd) {
 printf("%s\n", cmd);
 system(cmd);
}

bool buildUniversal() {
 bool built = false;

 if(olderC("font")) {
  build("cc -c ../src/font.c -o ../build/font.o");
  built = true;
 }
 
 if(olderC("pixelBuffer")) {
  build("cc -c ../src/pixelBuffer.c -o ../build/pixelBuffer.o");
  built = true;
 }
 
 if(olderC("graphic")) {
  build("cc -c ../src/graphic.c -o ../build/graphic.o");
  built = true;
 }

 if(olderC("content")) {
  build("cc -c ../src/content.c -o ../build/content.o");
  built = true;
 }

 if(older("be", "be.c")) {
  build("cc -c ../src/be.c -o ../build/be.o");
  built = true;
 }
 
 return built;
}

int main(void) {
 bool depsRebuilt = buildUniversal();
 
 #ifdef BE_DEBUG
  char* cmd = "cc ../build/*.o -g -Wextra -Wall -Werror --std=c89 -ansi -o ../build/be -lX11";
 #else
 
  #if BE_TARGET == x11
   char* cmd = "cc ../build/*.o -o ../build/be -lX11";
  #elif BE_TARGET == gtk
   char* cmd = "cc ../build/*.o -o be `pkg-config --cflags --libs gtk+-3.0`";   
  #endif

 #endif

 if(depsRebuilt) {
  build(cmd);
 }
 
 return 0;
}
