#ifndef FEEDER_H
#define FEEDER_H
#include "textStruct.h"
#include "feedingScheduleStruct.h"

void runningFeederMenu(feederSystem* feeder);
void displayFeederMenu(const textData* feederMenuTexts, const int* currentOption);

textData* setupFeederMenuTexts();
feederSystem* setupFeederSystem();

int runFeederMenuOption(feederSystem* feeder, int currentOption);

void feedNow();
void feederOperatingModeAuto(feederSystem* feeder);
void feederOperatingModeSkipNextFeed(feederSystem* feeder);
void feederOperatingModePause(feederSystem* feeder);

void navigateFeederMenu(const textData* feederMenuTexts, int* currentOption);

void displayNextFeedTime(textData* nextFeedTimeText, feederSystem* feeder);
void displayTotalNumberOfFeeds(textData* startMenuTexts, feederSystem* feeder);
void displayOperatingMode(textData* startMenuTexts, feederSystem* feeder);

int checkFeederSchedule(feederSystem* feeder);
int isFeedingTime(feedingSchedule* feederSchedule);
void checkOperatingMode(feedingSchedule* nextFeedingSchedule, feederSystem* feeder, operatingEnum operatingMode);
#endif // FEEDER_H
