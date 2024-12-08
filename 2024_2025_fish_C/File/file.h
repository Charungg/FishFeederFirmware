#ifndef FILE_H
#define FILE_H
#include "dateTimeStruct.h"

void saveSystemDateTime(systemDateTime* systemDateTime);
void writeSystemDateTimeToFile(systemDateTime* systemDateTime);

void loadSystemDateTimeSave();
void readSystemDateTime(char* lineText);
void splitDateTimeString(char* timeString, const char* delimiter, int* dateTimeArray, int* currentFreeIndex);
void removeZerosPadding(char* value);
void loadClock(const int* dateTimeArray);

void saveScheduleTime(systemTime* scheduleTime, int* numberOfFeeds, int* currentDay);
void writeScheduleTimeToFile(const systemTime* scheduleTime, const int* numberOfFeeds, const int* currentDay);



int isSystemTimeSaveExist();
int isScheduleTimeSaveExist();

void getNextTextLine(char* lineText,FILE* filePtr);

#endif // FILE_H
