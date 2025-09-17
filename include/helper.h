#ifndef BE_HELPER_H
#define BE_HELPER_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#include <stdbool.h>

bool isSourceNewer(char* source, char* result);

/* BE_HELPER_H */
#endif
