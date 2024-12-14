#ifndef FEEDER_H
#define FEEDER_H
#include "textStruct.h"
#include "feedingScheduleStruct.h"

void runningFeederMenu(feederSystem* feedingData);
void displayFeederMenu(const textData* feederMenuTexts, const int* currentOption);

textData* setFeederMenuTexts();
feederSystem* setFeederScheduleData();

int runFeederMenuOption(feederSystem* feedingData, int currentOption);

void feedNow();
void feederOperatingModeAuto(feederSystem* feederData);
void feederOperatingModeSkipNextFeed(feederSystem* feederData);
void feederOperatingModePause(feederSystem* feederData);

void navigateFeederMenu(const textData* feederMenuTexts, int* currentOption);

void displayNextFeedTime(textData* nextFeedTimeText, feederSystem* feedingScheduleData);
void displayTotalNumberOfFeeds(textData* startMenuTexts, feederSystem* feedingScheduleData);
void displayOperatingMode(textData* startMenuTexts, feederSystem* feedingScheduleData);

void checkFeederSchedule(feederSystem* feeder);
int isFeedingTime(feedingSchedule* feederSchedule);
#endif // FEEDER_H
