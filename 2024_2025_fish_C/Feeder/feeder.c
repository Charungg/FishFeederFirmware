#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fish.h"
#include "feeder.h"
#include "menu.h"
#include "userControl.h"


#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80

char nextFeedTime[LINE_SIZE];
int numberOfFeeds;

// A = Auto
// P = Paused
// S = Skip Next Feed
char operatingMode;

void runningFeederMenu(){
    int running_feeder_menu = 1; // continue flag
    int AFKTimer = 0;
    int prev_second = clockSecond();
    int currentOption = 0;

    textData* feederMenuTexts = setFeederMenuTexts();
    displayFeederMenu(feederMenuTexts,&currentOption);
    printf("Short press the button to navigate, long to confirm\n");

    do{
        // check for the button state every half second
        msleep(500L);
        char* result = buttonState(); // get the button state from the JavaFX application


        // Blanks the display after 1 minute of the button no being pressed
        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displayFeederMenu(feederMenuTexts,&currentOption);
            }
            displayClockTime();
        }

         if (isLongPressed(result, &AFKTimer)) {
            running_feeder_menu = runFeederMenuOption(currentOption);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        if (isShortPressed(result, &AFKTimer)) {
            navigateFeederMenu(feederMenuTexts, &currentOption);
        }

    }while(running_feeder_menu);
}

textData* setFeederMenuTexts(){
    int n = 5;
    textData* feederMenuTexts = malloc(sizeof(textData) * n);
    feederMenuTexts[0] = setupTextData(0,0,"Feed Now",1,"white","black");
    feederMenuTexts[1] = setupTextData(0,1,"Auto Feed",1,"white","black");
    feederMenuTexts[2] = setupTextData(0,2,"Skip Next Feed",1,"white","black");
    feederMenuTexts[3] = setupTextData(0,3,"Pause",1,"white","black");
    feederMenuTexts[4] = setupTextData(0,5,"Exit",1,"red","black");

    return feederMenuTexts;
}


void displayFeederMenu(textData* feederMenuTexts, int* currentOption){
    displayColour("white","black");
    displayClear();

    for (int i = 0; i < 5; i++){
        displayOptionText(&feederMenuTexts[i],*currentOption == i);
    }


    displayClockTime(clockSecond());
}


int runFeederMenuOption(int currentOption){

    switch(currentOption){
        case 0:
            feedNow();
            break;

        case 1:
            printf("Auto Feed Running\n");
            break;

        case 2:
            printf("Skip Next Feed Running\n");
            break;

        case 3:
            printf("Pause Running\n");
            break;

        case 4:
            return 0;

        default:
            exit(1);
    }

    return 1;
}


void feedNow(){
    printf("Short press to stop manual feeder\n");

    // step the motor through a complete rotation (360 degrees)
    for (int i = 0; i < 360; i++) {
        motorStep(); // step the motor in the JavaFX application
        msleep(30L); // can't go too fast, pause some milliseconds
    }

    numberOfFeeds+=1;
}


void navigateFeederMenu(textData* feederMenuTexts, int* currentOption){
    int newCurrentOption = ((*currentOption) + 1) % 5;

    displayOptionText(&feederMenuTexts[*currentOption],0);
    displayOptionText(&feederMenuTexts[newCurrentOption],1);

    *currentOption = newCurrentOption;
}


