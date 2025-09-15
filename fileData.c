#include <stdlib.h>
#include <stdio.h>

typedef struct FileData FileData;
typedef struct FileData{
 char* data;
 FileData* prev; 
 FileData* next;
} FileData;

FileData* initFileData(char* data) {
 FileData* res = malloc(sizeof(FileData));

 res->data = data;
 res->prev = NULL;
 res->next = NULL;

 return res;
}


FileData* readFile(char* filename) {
 FILE* file =  fopen(filename, "r");
 /*

 while
 char* data = 
 while(
 */
 
 fclose(file);
 return NULL;
}

