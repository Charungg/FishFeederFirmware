#ifndef SETUPTIME_H
#define SETUPTIME_H
#include "timeStruct.h"

void runningSetupTime();
void displaySetupTime(systemDateTime time);

void displaySystemDateTime(systemDateTime time);

void displayDay(const char* dayFormat, systemDateTime time, int isSelected);
void displayMonth(const char* dayFormat, systemDateTime time, int isSelected);
void displayYear(const char* dayFormat, systemDateTime time, int isSelected);

int selectDateTimeOption(int* currentOption, systemDateTime* time);
void runningModifyDateTimeValue(void (*incrementFunction)(systemDateTime*),systemDateTime* time, int* currentOption);

void incrementDay(systemDateTime* time);
void incrementMonth(systemDateTime* timed);
void incrementYear(systemDateTime* time);

void clearDisplaySelected();

int navigateSetupTime(int currentOption, systemDateTime time);

#endif //SETUPTIME_H
