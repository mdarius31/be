#include <stdio.h>
#include <stdlib.h>

int main(void) {
 char* cmd = "cc be.c -g -Wextra -Wall -Werror --std=c89 -ansi -o be -lX11";
 printf("%s\n", cmd);
 system(cmd);
 
 return 0;
}
