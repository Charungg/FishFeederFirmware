#ifndef SETUPSCHEDULETIME_H
#define SETUPSCHEDULETIME_H
#include "dateTimeStruct.h"
#include "textStruct.h"

void runningSetupScheduleTime();
void displaySetupScheduleTimeTexts(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay);

textData* setupDayTexts();
textData* setupScheduleTimeTexts();

int selectSetupScheduleTimeOption(textData* scheduleTimeTexts, textData* dayTexts, systemDateTime* systemDateTime, int* currentOption, int* currentDay);
void runningModifyDayText(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay);
void runningModifyNumberOfFeedText(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay);
void updateNumberOfFeed(textData* scheduleTimeTexts);

void navigateSetupScheduleTime(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay);
#endif //SETUPSCHEDULETIME_H
