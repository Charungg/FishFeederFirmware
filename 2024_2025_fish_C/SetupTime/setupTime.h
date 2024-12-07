#ifndef SETUPTIME_H
#define SETUPTIME_H
#include "dateTimeStruct.h"
#include "textStruct.h"

void runningSetupTime(void);
void displaySetupTime(systemDateTime* systemDateTime, textData* setupTimeTexts, int* currentOption);
textData* setupTimeMenuTexts();
void displaySystemDateTime(systemDateTime* systemDateTime, int* currentOption);

int selectDateTimeOption(int* currentOption, textData* setupTimeTexts, systemDateTime* dateTime);
void runningModifyDateTimeValue(void (*incrementFunction)(systemDateTime*),textData* setupTimeTexts, systemDateTime* time, int* currentOption);

void clearDisplaySelected();

int navigateSetupTime(systemDateTime* time, textData* setupTimeTexts, int currentOption);

#endif //SETUPTIME_H
