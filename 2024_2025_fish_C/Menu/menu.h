#ifndef MENU_H
#define MENU_H
#include "textStruct.h"
#include "lineStruct.h"

void introduction(void);
void displayStartMenu(textData* startMenuTexts);
void displayClockTime();
void runningStartMenu(char* title);
textData* setupStartMenuTexts(char* title);
lineData setupLineData(int xOrigin, int yOrigin, int xEnd, int yEnd);

void updateNextFeedTime(textData* nextFeedTimeText);
void updateNumberOfFeeds(textData* startMenuTexts);
void updateOperatingMode(textData* startMenuTexts);

int isTimeUpdated(int* prev_second);
int isUserAFK(int *AFKTimer);
void setAFKTimerToZero(int *AFKTimer);
void blankDisplay();
void isOptionSelected(const int* isSelected);

void displayMainMenu(textData* userMainMenuOptions, int currentOption);
void runningMainMenu();
void navigateTexts(textData* mainMenuOptions, int* currentOption);
int runMainMenuOption(int currentOption);

textData* setupMainMenuTexts();
textData setupTextData(int xAxis, int yAxis, char* text, int size, char* foregroundColour, char* backgroundColor);

void displayOptionText(const textData* textData, int isSelected);
void isTextSelected(const textData* textData, int isSelected);
#endif // MENU_H
