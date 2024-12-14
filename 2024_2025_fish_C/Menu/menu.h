#ifndef MENU_H
#define MENU_H
#include "textStruct.h"
#include "lineStruct.h"
#include "feedingScheduleStruct.h"

void introduction(void);
void displayStartMenu(textData* startMenuTexts,feederSystem* feedingScheduleData);
void displayClockTime();
void runningStartMenu(char* title);

textData* setupStartMenuTexts(char* title);
lineData setupLineData(int xOrigin, int yOrigin, int xEnd, int yEnd);

int isTimeUpdated(int* prev_second);
int isUserAFK(int *AFKTimer);
void setAFKTimerToZero(int *AFKTimer);
void blankDisplay();
void isOptionSelected(const int* isSelected);



void displayMainMenu(const textData* userMainMenuOptions, int currentOption);
void runningMainMenu(feederSystem* feedingScheduleData);

void navigateTexts(const textData* mainMenuOptions, int* currentOption);
int runMainMenuOption(feederSystem* feedingScheduleData, int currentOption);

textData* setupMainMenuTexts();
textData setupTextData(int xAxis, int yAxis, char* text, int size, char* foregroundColour, char* backgroundColor);

void displayOptionText(const textData* textData, int isSelected);
void isTextSelected(const textData* textData, int isSelected);
#endif // MENU_H
