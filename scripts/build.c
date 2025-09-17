#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/helper.c"

#define BE_BUILD_SCRIPT
#include "../include/config.h"


#define isSourceNewerC(dep) isSourceNewer("./src/" dep ".c", "./build/" dep ".o") || isSourceNewer("./include/" dep ".h", "./build/" dep ".o")

#define basicBuildCmd(dep) "cc -c ./src/" dep ".c -o ./build/" dep ".o"
#define basicBuild(dep) build(basicBuildCmd(dep))

void build(char* cmd) {
 printf("%s\n", cmd);
 system(cmd);
}

bool buildUniversalDeps() {
 bool built = false;
 
 if(isSourceNewerC("font")) {
  basicBuild("font");
  built = true;
 }
 
 if(isSourceNewerC("pixelBuffer")) {
  basicBuild("pixelBuffer");
  built = true;
 }
 
 if(isSourceNewerC("graphic")) {
  basicBuild("graphic");
  built = true;
 }

 if(isSourceNewerC("content")) {
  basicBuild("content");
  built = true;
 }

 if(isSourceNewer("./src/be.c", "./build/be")) {
  build("cc -c ./src/be.c -o ./build/be.o");
  built = true;
 }
 
 return built;
}

int main(void) {
 if(isSourceNewer("./scripts/build.c", "./scripts/build")) {
  printf("Rebuilding the build system\n");
  build("cc ./scripts/build.c -o ./scripts/build");
  system("./scripts/build");
  return 0;
 }

 bool depsRebuilt = buildUniversalDeps();
 
 #ifdef BE_DEBUG
  char* cmd = "cc ./build/*.o -g -Wextra -Wall -Werror --std=c89 -ansi -o ./build/be -lX11";
 #else
 
  #if BE_TARGET == x11
   char* cmd = "cc ./build/*.o -o ./build/be -lX11";
  #elif BE_TARGET == gtk
   char* cmd = "cc ./build/*.o -o be `pkg-config --cflags --libs gtk+-3.0`";   
  #endif

 #endif

 if(depsRebuilt) {
  build(cmd);
 }
 
 return 0;
}
