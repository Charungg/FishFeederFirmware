#ifndef SETUPTIME_H
#define SETUPTIME_H
#include "dateTimeStruct.h"
#include "feedingScheduleStruct.h"
#include "textStruct.h"

void runningSetupTimeMenu(feederSystem* feeder);
void displaySetupTime(const systemDateTime* systemDateTime, const textData* setupTimeTexts, const int* currentOption);
textData* setupTimeMenuTexts();
void displaySystemDateTime(const systemDateTime* systemDateTime, const int* currentOption);

int selectSetupTimeOption(int* currentOption, const textData* setupTimeTexts, systemDateTime* dateTime, feederSystem* feeder);
void runningModifyDateTimeValue(void (*incrementFunction)(systemDateTime*), const textData* setupTimeTexts, systemDateTime* time, int* currentOption, feederSystem* feeder);

int navigateSetupTime(const systemDateTime* time, const textData* setupTimeTexts, int currentOption);

#endif //SETUPTIME_H
