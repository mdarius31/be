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

bool exists(char* name) {
#ifdef _WIN32
 WIN32_FILE_ATTRIBUTE_DATA nameInfo;

 if (!GetFileAttributesEx(name, GetFileExInfoStandard, &nameInfo)) {
  return false;
 }
#else
 struct stat nameStat;
 
 if(stat(name, &nameStat) != 0) {
  return false;
 }
#endif
 return true;
}