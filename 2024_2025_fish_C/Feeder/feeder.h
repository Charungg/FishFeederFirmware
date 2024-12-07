#ifndef FEEDER_H
#define FEEDER_H
#include "textStruct.h"

void runningFeederMenu();
void displayFeederMenu(textData* feederMenuTexts, int* currentOption);
textData* setFeederMenuTexts();

int runFeederMenuOption(int currentOption);
void navigateFeederMenu(textData* feederMenuTexts, int* currentOption);

void feedNow();
#endif // FEEDER_H
