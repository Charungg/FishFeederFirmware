#ifndef FILE_H
#define FILE_H
#include "dateTimeStruct.h"
#include "feedingScheduleStruct.h"

void saveSystemDateTime(systemDateTime* systemDateTime);
void writeSystemDateTimeToFile(systemDateTime* systemDateTime);

void loadSystemDateTimeSave();
systemDateTime getSystemDateTimeFromFile();



void saveScheduleTime(feederSystem* scheduleData);
void writeScheduleTimeToFile(feedingSchedule* scheduleData);

feederSystem* loadScheduleTime();
feederSystem* getScheduleDataFromFile();

int getAmountOfSchedules();




// Both functionality

int isSystemTimeSaveExist();
int isScheduleTimeSaveExist();

systemDate* dateStringToSystemDate(char* dateString);
systemTime* timeStringToSystemTime(char* timeString);

void removeZeroPadding(char* value);
void getNextLineText(char* lineText,FILE* filePtr, int amountOfRead);

void loadClock(systemDateTime* systemDateTime);


#endif // FILE_H
