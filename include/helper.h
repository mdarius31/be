#ifndef BE_HELPER_H
#define BE_HELPER_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#endif

#include <stdbool.h>

bool older(const char *file1, const char *file2);

/* BE_HELPER_H */
#endif
