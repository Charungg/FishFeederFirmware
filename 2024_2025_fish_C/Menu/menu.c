#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "menu.h"

#include <string.h>

#include "feeder.h"
#include "userControl.h"
#include "textStruct.h"
#include "lineStruct.h"
#include "setUpTime.h"
#include "setupScheduleTime.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80

extern char nextFeedTime[];
extern int numberOfFeeds;

// A = Auto
// P = Paused
// S = Skip Next Feed
extern char operatingMode;

void runningStartMenu(char* title){
    int running_start_menu = 1; // continue flag
    int AFKTimer = 0;
    int prev_second = clockSecond();

    textData* startMenuTexts = setupStartMenuTexts(title);
    displayStartMenu(startMenuTexts);
    printf("Short press to enter main menu, Long press the button to quit\n");

    do{
        // Check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        // Blanks the display after 1 minute of the button no being pressed
        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displayStartMenu(startMenuTexts);
            }
            displayClockTime();
        }

        if (isLongPressed(result, &AFKTimer)) {
            running_start_menu = 0;
        }

        // check the result and quit this look if the button is short pressed
        if (isShortPressed(result, &AFKTimer)){
            runningMainMenu();
            displayStartMenu(startMenuTexts);
        }
    }while(running_start_menu);
}

textData* setupStartMenuTexts(char* title){
    const int n = 4;
    textData* textDataPtr = malloc(sizeof(textData)*n);
    textDataPtr[0] = setupTextData(0,0,title,2,"#8A2BE2","black");
    textDataPtr[1] = setupTextData(0, 2, "Next Feed Time:", 1, "white", "black");
    textDataPtr[2] = setupTextData(0, 3, "Number Of Feeds:", 1, "white", "black");
    textDataPtr[3] = setupTextData(0, 4, "Operating Mode:", 1, "white", "black");
    return textDataPtr;
}


void updateNextFeedTime(textData* startMenuTexts){
    char nextFeedTimeText[] = "Next Feed Time: ";
    strcat(nextFeedTimeText, nextFeedTime);

    startMenuTexts[1].text = malloc(sizeof(nextFeedTimeText));
    strcpy(startMenuTexts[1].text, nextFeedTimeText);
}


void updateNumberOfFeeds(textData* startMenuTexts){
    char numberOfFeedsText[] = "Number Of Feed: ";
    char text[LINE_SIZE];

    sprintf(text, "%d", numberOfFeeds);
    strcat(numberOfFeedsText, text);

    startMenuTexts[2].text = malloc(sizeof(numberOfFeedsText));
    strcpy(startMenuTexts[2].text, numberOfFeedsText);
}

void updateOperatingMode(textData* startMenuTexts){
    char operatingModeText[] = "Operating Mode: ";
    char text[LINE_SIZE];

    sprintf(text, "%c", operatingMode);
    printf("%s\n", operatingModeText);
    printf("%c\n", operatingMode);


    strcat(operatingModeText, text);
    startMenuTexts[3].text = malloc(strlen(operatingModeText)+1);
    strcpy(startMenuTexts[3].text, operatingModeText);
}




lineData setupLineData(int xOrigin, int yOrigin, int xEnd, int yEnd){
    lineData newLineData;
    newLineData.xOrigin = xOrigin;
    newLineData.yOrigin = yOrigin;
    newLineData.xEnd = xEnd;
    newLineData.yEnd = yEnd;
    return newLineData;
}


void displayStartMenu(textData* startMenuTexts){
    displayColour("white", "black");
    displayClear();

    updateNextFeedTime(startMenuTexts);
    updateNumberOfFeeds(startMenuTexts);
    updateOperatingMode(startMenuTexts);

    for (int i=0; i<4; i++){
        displayOptionText(&startMenuTexts[i],0);
    }
}


void displayClockTime(){
    // display graphic lines around time
    displayColour("white", "black");
    displayLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1);
    displayLine(0, SCREEN_HEIGHT - CHAR_HEIGHT*2-1, SCREEN_WIDTH, SCREEN_HEIGHT - CHAR_HEIGHT*2-1);

    // display the time value
    char time[LINE_SIZE];
    snprintf(time, LINE_SIZE, "%02i-%02i-%02i", clockHour(), clockMinute(), clockSecond());
    displayText(SCREEN_WIDTH/2-4*CHAR_WIDTH, SCREEN_HEIGHT-CHAR_HEIGHT*1.5, time, 1);
}


int isTimeUpdated(int* prev_second) {
    int second = clockSecond();
    if (second != *prev_second) {
        *prev_second = second;
        return 1;
    }
    return 0;
}


int isUserAFK(int *AFKTimer){
    *AFKTimer += 1;
    // printf("AFK Timer: %d\n", *AFKTimer);
    if (*AFKTimer >= 60) {
        blankDisplay();
        *AFKTimer = 0;
        return 1;
    }
    return 0;
}


void runningMainMenu(){
    int running_main_menu= 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();
    int currentOption = 0;

    textData* mainMenuTexts = setupMainMenuTexts();
    displayMainMenu(mainMenuTexts, currentOption);
    // printf("Short press the button to navigate, long to confirm\n");

    do
    {
        // check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displayMainMenu(mainMenuTexts, currentOption);
            }
        }

        if (isLongPressed(result, &AFKTimer)) {
            running_main_menu = runMainMenuOption(currentOption);
            displayMainMenu(mainMenuTexts, currentOption);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result, &AFKTimer)) {
            navigateTexts(mainMenuTexts, &currentOption);
        }

    }while(running_main_menu);
}


textData* setupMainMenuTexts(){
    int n = 4;
    textData* textDataPtr = malloc(sizeof(textData)*n);
    textDataPtr[0] = setupTextData(0,0,"Feeder Menu", 1,"white","black");
    textDataPtr[1] = setupTextData(0,2,"Setup Time", 1,"white","black");
    textDataPtr[2] = setupTextData(0,4,"Setup Schedule Time", 1,"white","black");
    textDataPtr[3] = setupTextData(0,7,"Exit", 1,"red","black");
    return textDataPtr;
}

textData setupTextData(int xAxis, int yAxis, char* text, int size, char* foregroundColour, char* backgroundColor){
    textData newTextData;
    newTextData.xAxis = xAxis;
    newTextData.yAxis = yAxis;
    newTextData.text = text;
    newTextData.size = size;

    newTextData.foregroundColour = foregroundColour;
    newTextData.backgroundColour = backgroundColor;

    return newTextData;
}


void displayMainMenu(textData* textDataArray, int currentOption){
    displayColour("white", "black");
    displayClear();

    for (int i=0; i<4; i++){
        if (i == currentOption){
            displayOptionText(&textDataArray[i],1);
        }

        else{
            displayOptionText(&textDataArray[i],0);
        }
    }
}



int runMainMenuOption(int currentOption){
    switch (currentOption) {
        case 0:
            runningFeederMenu();
            break;

        case 1:
            runningSetupTime();
            break;

        case 2:
            runningSetupScheduleTime();
            break;

        case 3:
            return 0;

        default:
            exit(1);
    }

    return 1;
}


void navigateTexts(textData* mainMenuOptions, int* currentOption){
    int newCurrentOption = ((*currentOption) + 1) % 4;

    displayOptionText(&mainMenuOptions[*currentOption],0);
    displayOptionText(&mainMenuOptions[newCurrentOption],1);

    *currentOption = newCurrentOption;
}


void displayOptionText(const textData* textData, int isSelected){
    isTextSelected(textData, isSelected);
    displayText(CHAR_WIDTH*textData -> xAxis, CHAR_HEIGHT*textData -> yAxis, textData -> text, textData -> size);
}


void isTextSelected(const textData* textData, int isSelected){
    if (isSelected) {
        displayColour("black", "white");
    }

    else{
        displayColour(textData -> foregroundColour, textData -> backgroundColour);
    }
}


void blankDisplay(){
    int running_blank_display = 1;
    char* result;

    displayColour("white", "grey");
    displayClear();

    do{
        msleep(500L);
        result = buttonState();

        if (strcmp(result, "LONG_PRESS") == 0 || strcmp(result, "SHORT_PRESS") == 0){
            running_blank_display = 0;
        }

    }while(running_blank_display);
}

