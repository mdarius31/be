#include "../include/helper.h"

bool older(const char *file1, const char *file2) {
#ifdef _WIN32
 WIN32_FILE_ATTRIBUTE_DATA fileInfo1, fileInfo2;

 if (!GetFileAttributesEx(file1, GetFileExInfoStandard, &fileInfo1) ||
  !GetFileAttributesEx(file2, GetFileExInfoStandard, &fileInfo2)) {
  return true;
 }

 return CompareFileTime(&fileInfo1.ftLastWriteTime, &fileInfo2.ftLastWriteTime) < 0;
#else
 struct stat stat1, stat2;

 if (stat(file1, &stat1) != 0 || stat(file2, &stat2) != 0) {
  return true;
 }

 return stat1.st_mtime < stat2.st_mtime;
#endif
}
