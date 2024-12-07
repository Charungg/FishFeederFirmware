#ifndef SETUPSCHEDULETIME_H
#define SETUPSCHEDULETIME_H
#include "dateTimeStruct.h"
#include "textStruct.h"

void runningSetupScheduleTime();
void displaySetupScheduleTimeTexts(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay);

textData* setupDayTexts();
textData* setupScheduleTimeTexts();

void navigateSetupScheduleTime(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay);

#endif //SETUPSCHEDULETIME_H
