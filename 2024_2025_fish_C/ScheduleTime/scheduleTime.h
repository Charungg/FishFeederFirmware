#ifndef SETUPSCHEDULETIME_H
#define SETUPSCHEDULETIME_H
#include "dateTimeStruct.h"
#include "textStruct.h"
#include "feedingScheduleStruct.h"

void runningSetupScheduleTime(feederSystem* feedingScheduleData);
void displaySetupScheduleTimeTexts(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* scheduleData, const int* currentOption);
void displayNumberOfFeedText(const feedingSchedule* scheduleData, int isSelected);

textData* setupDayTexts();
textData* setupScheduleTimeTexts();
feedingSchedule* setupFeedingSchedule();

int selectSetupScheduleTimeOption(textData* scheduleTimeTexts, textData* dayTexts, feederSystem* feedingScheduleData, feedingSchedule* scheduleData, int* currentOption);

void runningModifyDayOfTheWeekText(const textData* scheduleTimeTexts, textData* dayTexts, feedingSchedule* scheduleData, const int* currentOption);
void updateDayOfTheWeek(feedingSchedule* scheduleData, textData* dayTexts);

void runningModifyNumberOfFeedText(const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* scheduleData, const int* currentOption);
void updateNumberOfFeed(feedingSchedule* scheduleData);

void runningModifyScheduleTimeText(void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime), textData* scheduleTimeTexts, textData* dayTexts, feedingSchedule* scheduleData, int* currentOption);
void incrementScheduleHour(systemTime* systemTime);
void incrementScheduleMinute(systemTime* systemTime);
void incrementScheduleSecond(systemTime* systemTime);

void navigateSetupScheduleTime(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* scheduleData, int* currentOption);

#endif //SETUPSCHEDULETIME_H
