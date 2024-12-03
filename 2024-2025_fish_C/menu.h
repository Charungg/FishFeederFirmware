#ifndef MENU_H
#define MENU_H

void introduction(void);
void displayStartMenu(char* title);
void displayClockTime();
void runningStartMenu(char* title);

int isTimeUpdated(int* prev_second);
int isUserAFK(int* AFKTimer);
void blankDisplay();
void isOptionSelected(int isSelected);

void displayMainMenu();
void runningMainMenu();
int navigateMainMenu(int currentOption);
int runMainMenuOption(int currentOption, char* result);

void displayExit(int isSelected, int yAxis);
#endif // MENU_H
