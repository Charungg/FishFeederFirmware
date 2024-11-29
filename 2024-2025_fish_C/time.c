//
// Created by charl on 29/11/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fish.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 64/8 = 8 HEIGHT
// 128/6 = 21.333^* WIDTH

#define LINE_SIZE 80

void displaySetupTime(systemTime time) {
    displayColour("white", "black");
    displayClear();

    displaySystemTime(time);
    displaySystemDate(time);

    displayConfirm(0);
    displayExit(0,7);
}


void displaySystemDate(systemTime time) {
    char text[LINE_SIZE];
    char* dateFormat = "Date |%02d:%02d:%04d";
    displayColour("white", "grey");

    snprintf(text, LINE_SIZE, dateFormat, time.day, time.month, time.year);
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*2,text,1);
}

void displaySystemTime(systemTime time) {
    char text[LINE_SIZE];
    char* timeFormat = "Time |%02d:%02d:%02d";
    displayColour("white", "grey");

    snprintf(text, LINE_SIZE, timeFormat, time.hour, time.minute, time.second);
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*4,text,1);
}


systemTime getSystemTime() {
    systemTime time;
    time.day = clockDay();
    time.month = clockMonth();
    time.year = clockYear();
    time.hour = clockHour();
    time.minute = clockMinute();
    time.second = clockSecond();
    return time;
}

void runningSetupTime(char* result) {
    int running_setup_time = 1;
    int currentOption = 0;


    printf("Short press to navigate, long press to change value\n");
    systemTime time = getSystemTime();
    displaySetupTime(time);

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if (strcmp(result, "LONG_PRESS") == 0) {

            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (strcmp(result, "SHORT_PRESS") == 0) {
            // currentOption = navigateSetupTime();
        }

    }while(running_setup_time);
}


void displayConfirm(int isSelected) {
    if (isSelected) {
        displayColour("black", "white");
    }
    else {
        displayColour("white", "black");
    }

    displayText(CHAR_WIDTH*14,CHAR_HEIGHT*7,"Confirm",1);
}



// int navigateSetupTime(int currentOption) {
//     switch(currentOption) {
//         case ()
// }


