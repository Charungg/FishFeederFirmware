#ifndef SETUPSCHEDULETIME_H
#define SETUPSCHEDULETIME_H
#include "dateTimeStruct.h"
#include "textStruct.h"
#include "feedingScheduleStruct.h"

// Display and runs the setup schedules time menu.
void runningSetupScheduleTimeMenu(feederSystem* feeder);
void displaySetupScheduleTimeTexts(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* scheduleData, const int* currentOption);
void displayNumberOfFeedText(const feedingSchedule* scheduleData, int isSelected);

// Setup schedule time menu texts.
// Such as day texts, schedules time texts like Exit and Save
// and also a default feeding schedules if save text file doesn't contain any feeding schedules.
textData* setupDayTexts();
textData* setupScheduleTimeTexts();
feedingSchedule* setupFeedingSchedule();

// Allow the user to modify the values on the schedules time menu.
int selectSetupScheduleTimeOption(const textData* scheduleTimeTexts, const textData* dayTexts, feederSystem* feedingScheduleData, feedingSchedule* scheduleData, const int* currentOption);

// User can modify and increment day of the week text from Monday - Sunday and feeding schedules day of week data.
void runningModifyDayOfTheWeekText(const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* schedule, const int* currentOption, feederSystem* feeder);
void incrementDayOfTheWeek(feedingSchedule* schedule, const textData* dayTexts);

// User can modify and increment number of feed per feeding schedule.
void runningModifyNumberOfFeedText(const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* scheduleData, const int* currentOption, feederSystem* feeder);
void incrementNumberOfFeed(feedingSchedule* scheduleData);

// User can modify and increment the hour, minute and second of the feeding schedule.
void runningModifyScheduleTimeText(void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime), const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* scheduleData, const int* currentOption, feederSystem* feeder);
void incrementScheduleHour(systemTime* systemTime);
void incrementScheduleMinute(systemTime* systemTime);
void incrementScheduleSecond(systemTime* systemTime);

// Allows the user to navigate in setup schedules time menu.
void navigateSetupScheduleTime(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* scheduleData, int* currentOption);

#endif //SETUPSCHEDULETIME_H
