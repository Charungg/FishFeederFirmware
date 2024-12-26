#ifndef MENU_H
#define MENU_H
#include "textStruct.h"
#include "feedingScheduleStruct.h"

// Displays and runs the program start menu.
void introduction(void);
void displayStartMenu(textData* startMenuTexts, const feederSystem* feeder);
void displayClockTime();
void runningStartMenu(char* title);

// Setup start menu texts.
textData* setupStartMenuTexts(char* title);

// Functions to keep track whenever system time has been updated.
int isTimeUpdated(int* prev_second);

// Checks if the user has been AFK (away from keyboard) for more than 60 second.
// If so blanks the display to prevent OLEDs screen burn in.
int isUserAFK(int *AFKTimer, feederSystem* feeder);
void blankDisplay(feederSystem* feeder);

// Display and runs the program main menu to allow users to go
// feeder menu, setup time menu or setup schedule time menu.
void displayMainMenu(const textData* userMainMenuOptions, int currentOption);
void runningMainMenu(feederSystem* feeder);

// Setup main menu texts.
textData* setupMainMenuTexts();
textData setupTextData(int xAxis, int yAxis, char* text, int size, char* foregroundColour, char* backgroundColor);

// Allows the user to navigate and run the main menu operations.
void navigateMainMenuTexts(const textData* mainMenuOptions, int* currentOption);
int runMainMenuOption(feederSystem* feeder, int currentOption);

// Displays every menu texts and their colours are dependent if the user is selected that text.
void displayOptionText(const textData* textData, int isSelected);
void isTextSelected(const textData* textData, int isSelected);
#endif // MENU_H
