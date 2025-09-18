#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../src/helper.c"

#define BE_BUILD_SCRIPT
#include "../include/config.h"


#define shouldBuildObj(dep) \
 isSourceNewer("./src/" dep ".c", "./build/" dep ".o") ||\
 isSourceNewer("./include/" dep ".h", "./build/" dep ".o")

#define buildObj(dep) build("cc -c ./src/" dep ".c -o ./build/" dep ".o")


#define tryBuildObj(dep) \
 if(shouldBuildObj(dep)) {\
  res.tried = true;\
  printf("Trying to build %s\n", dep);\
  res.good = buildObj(dep) == 0;\
  if(!res.good) {\
   printf("Failed to build %s\n\n", dep);\
   return res;\
  }\
  printf("Built %s\n\n", dep);\
 }\

int build(char* cmd) {
 printf("%s\n", cmd);
 return system(cmd);
}

typedef struct {
 bool tried;
 bool good;
} BuildResult;

BuildResult buildDeps() {
 BuildResult res = {false, false};

 tryBuildObj("font");
 tryBuildObj("pixelBuffer");
 tryBuildObj("graphic");
 tryBuildObj("content");

 if(res.tried && res.good && isSourceNewer("./src/be.c", "./build/be")) {
  res.good = build("cc -c ./src/be.c -o ./build/be.o") == 0;
 }

 return res;
}

int main(void) {
 if(isSourceNewer("./scripts/build.c", "./scripts/build")) {
  printf("Rebuilding the build script\n");

  if(build("cc ./scripts/build.c -o ./scripts/build") == 0) {
   printf("Executing new build script\n");
   system("./scripts/build");
  
  } else {
   printf("Failed to rebuild the build script");
   return -1;
  }

  return 0;
 }

 if(!exists("./build")) {
  printf("\"build\" directory dosent exists, attempting to create it\n\n");
  system("mkdir build");
  if(!exists("./build")) {
   printf("Failed to create \"build\" directory, please create it by hand and rerun\n");
   return -1;
  }
 }

 BuildResult depsRes = buildDeps();
 
 #ifdef BE_DEBUG
  char* cmd = "cc ./build/*.o -g -Wextra -Wall -Werror --std=c89 -ansi -o ./build/be -lX11";
 #else
 
  #if BE_TARGET == x11
   char* cmd = "cc ./build/*.o -o ./build/be -lX11";
  #elif BE_TARGET == gtk
   char* cmd = "cc ./build/*.o -o be `pkg-config --cflags --libs gtk+-3.0`";   
  #endif

 #endif

 bool finalRes = depsRes.tried ? depsRes.good : true;

 if(depsRes.tried && depsRes.good) {
  finalRes = build(cmd) == 0;
  printf("Finished!\n");
 } else if(depsRes.tried) {
  printf("An error occured\n");
 } else {
  printf("All up to date!\n");
 }
 
 return finalRes ? 0 : -1;
}
