#ifndef FILE_H
#define FILE_H
#include "dateTimeStruct.h"
#include "feedingScheduleStruct.h"

void saveSystemDateTime(systemDateTime* systemDateTime);
void writeSystemDateTimeToFile(systemDateTime* systemDateTime);

void loadSystemDateTimeSave();
systemDateTime getSystemDateTimeFromFile();

void saveScheduleTime(feedingSchedule* schedule);
void writeScheduleTimeToFile(feedingSchedule* scheduleData);

feederSystem* loadFeederSystem();
feedingSchedule* getScheduleDataFromFile();

int getAmountOfSchedules();

int isSystemDateTimeSaveExist();
int isScheduleTimeSaveExist();

systemDate* dateStringToSystemDate(char* dateString);
systemTime* timeStringToSystemTime(char* timeString);

void removeZeroPadding(char* value);
void getNextLineText(char* lineText,FILE* filePtr, int amountOfRead);

void loadClock(systemDateTime* systemDateTime);


#endif // FILE_H
