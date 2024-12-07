#ifndef FILE_H
#define FILE_H
#include "dateTimeStruct.h"

void saveSystemDateTime(systemDateTime* dateTime);
void loadSystemDateTime();
void readSystemDateTime(char* lineText);
void splitDateTimeString(char* timeString, const char* delimiter, int* dateTimeArray, int* currentFreeIndex);
void removeZerosPadding(char* value);
void loadClock(const int* dateTimeArray);
#endif // FILE_H
