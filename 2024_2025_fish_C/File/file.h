#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include "dateTimeStruct.h"
#include "feedingScheduleStruct.h"

// Save and write user new desired system date and time for the program.
void saveSystemDateTime(systemDateTime* systemDateTime);
void writeSystemDateTimeToFile(const systemDateTime* systemDateTime);

// Load the system date time from the save text file.
void loadSystemDateTimeSave();
systemDateTime getSystemDateTimeFromFile();

// Save and write user created feeding schedules for the feeder system.
void saveScheduleTime(feedingSchedule* schedule);
void writeScheduleTimeToFile(feedingSchedule* scheduleData);

// Load feeding schedules from the save text file.
feederSystem* loadFeederSystem();
feedingSchedule* getScheduleDataFromFile();

// Get the amount of schedules that exist within he save text file.
int getAmountOfSchedules();

// Check if either system date time or schedules save exist within the text file.
int isSystemDateTimeSaveExist();
int isScheduleTimeSaveExist();

// Convert save text files date and time data to their respective structs.
systemDate* dateStringToSystemDate(char* dateString);
systemTime* timeStringToSystemTime(char* timeString);

// Removes zero's padding from the date and time string.
void removeZeroPadding(char* value);

// Gets x line from the save text file.
void getNextLineText(char* lineText, FILE* filePtr, int amountOfRead);

// Load current date and time or if the save exist then the date and time saves from the text file.
void loadClock(const systemDateTime* systemDateTime);


#endif // FILE_H
