#ifndef SETUPTIME_H
#define SETUPTIME_H
#include "dateTimeStruct.h"
#include "feedingScheduleStruct.h"
#include "textStruct.h"

// Displays and runs the setup system time menu.
void runningSetupTimeMenu(feederSystem* feeder);
void displaySetupTime(systemDateTime* systemDateTime, textData* setupTimeTexts, int* currentOption);
void displaySystemDateTime(systemDateTime* systemDateTime, int* currentOption);

// Set up the time menu texts.
textData* setupTimeMenuTexts();

// Allows the user to select and run the time menu operations.
int selectSetupTimeOption(int* currentOption, textData* setupTimeTexts, systemDateTime* dateTime, feederSystem* feeder);

// User can modify and increment date time values such as day, month, year, hour, minute and second.
void runningModifyDateTimeValue(void (*incrementFunction)(systemDateTime*), textData* setupTimeTexts, systemDateTime* dateTime, int* currentOption, feederSystem* feeder);

// Allow the user to navigate in the setup time menu.
int navigateSetupTime(systemDateTime* time, textData* setupTimeTexts, int currentOption);

#endif //SETUPTIME_H
