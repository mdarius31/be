#include "../include/helper.h"

bool isSourceNewer(char* source, char* result) {
#ifdef _WIN32
 WIN32_FILE_ATTRIBUTE_DATA resultInfo, sourceInfo;

 if (!GetFileAttributesEx(result, GetFileExInfoStandard, &resultInfo) ||
  !GetFileAttributesEx(source, GetFileExInfoStandard, &sourceInfo)) {
  return true;
 }

 return CompareFileTime(&sourceInfo.ftLastWriteTime, &resultInfo.ftLastWriteTime) > 0;
#else
 struct stat resultStat, sourceStat;


 if (stat(result, &resultStat) != 0 || stat(source, &sourceStat) != 0) {
  return true;
 }


 return sourceStat.st_mtime > resultStat.st_mtime;
#endif
}