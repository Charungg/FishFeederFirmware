#ifndef FILE_H
#define FILE_H
#include "timeStruct.h"

void saveSystemDateTime(systemDateTime time);
void loadSystemDateTime();
void readSystemDateTime(char* lineText);
void splitDateTimeString(char* timeString, const char* delimiter, int* dateTimeArray, int* currentFreeIndex);
void removeZerosPadding(char* value);
void loadClock(const int* dateTimeArray);
#endif // FILE_H
