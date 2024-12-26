#ifndef FEEDER_H
#define FEEDER_H
#include "textStruct.h"
#include "feedingScheduleStruct.h"

// Runs and displays the feeder menu.
void runningFeederMenu(feederSystem* feeder);
void displayFeederMenu(const textData* feederMenuTexts, const int* currentOption);

// Setup feeder menu texts and setup default feeder system if there is no feeding schedules.
textData* setupFeederMenuTexts();
feederSystem* setupFeederSystem();

// Run feeder menu operations.
int runFeederMenuOption(feederSystem* feeder, int currentOption);

// Feeds the fishes in one full rotation of the motor.
void feedNow();

// Allows the user to change the feeder system operating mode.
void feederOperatingModeAuto(feederSystem* feeder);
void feederOperatingModeSkipNextFeed(feederSystem* feeder);
void feederOperatingModePause(feederSystem* feeder);

// Allows the user to navigate in the feeder menu.
void navigateFeederMenu(const textData* feederMenuTexts, int* currentOption);

// Display the feeder system information such as next feed time, total number of feed and operating mode.
void displayNextFeedTime(textData* nextFeedTimeText, const feederSystem* feeder);
void displayTotalNumberOfFeeds(textData* startMenuTexts, const feederSystem* feeder);
void displayOperatingMode(textData* startMenuTexts, const feederSystem* feeder);

// Checks if the feeder schedules is the same as current time.
// If so check the operating mode before feeing the fishes.
int checkFeederSchedule(feederSystem* feeder);
int isFeedingTime(feedingSchedule* feederSchedule);
void checkOperatingMode(const feedingSchedule* nextFeedingSchedule, feederSystem* feeder, operatingEnum operatingMode);
#endif // FEEDER_H
