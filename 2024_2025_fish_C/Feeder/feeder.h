#ifndef FEEDER_H
#define FEEDER_H
#include "textStruct.h"
#include "feedingScheduleStruct.h"

void runningFeederMenu(feedingScheduleData* feedingScheduleData);
void displayFeederMenu(const textData* feederMenuTexts, const int* currentOption);

textData* setFeederMenuTexts();
feedingScheduleData* setFeederScheduleData();

int runFeederMenuOption(int currentOption);

void feedNow();

void navigateFeederMenu(const textData* feederMenuTexts, int* currentOption);

void displayNextFeedTime(textData* nextFeedTimeText, feedingScheduleData* feedingScheduleData);
void displayTotalNumberOfFeeds(textData* startMenuTexts, feedingScheduleData* feedingScheduleData);
void displayOperatingMode(textData* startMenuTexts, feedingScheduleData* feedingScheduleData);

void checkFeederSchedule();
int isFeedingTime(feedingSchedule* feederSchedule);
int isFeederScheduleTheSame(feedingSchedule* feederSchedule, feedingSchedule* otherFeederSchedule);
#endif // FEEDER_H
