#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "menu.h"
#include "userControl.h"
#include "feeder.h"


#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80

// Global variable to keep track of how long is the user inactive.
// AFK = away from keyboard
int AFKTimer = 0;

// allow detection when seconds value has changed
int prev_sec = 0;


void runningStartMenu(char* title){
    int running_start_menu = 1; // continue flag

    displayStartMenu(title);
    printf("Short press to enter main menu, Long press the button to quit\n");

    do{
        // Check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        // Update time on display
        displayUpdatedTime();

        // Blanks the display after 1 minute of the button no being pressed
        if(isUserAFK(result)){
            displayStartMenu(title);
        }

        else if (isLongPressed(result)) {
            running_start_menu = 0;
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result)){
            logAdd(GENERAL, "main menu");
            AFKTimer = clockSecond();

            displayMainMenu();
            runningMainMenu();
            displayStartMenu(title);
            free(result);
        }

    }while(running_start_menu);
}


void displayStartMenu(char* title){
    // output banner (see fish.h for details)
    displayColour("white", "black"); // white text on black background
    displayClear(); // clear JavaFX display.
    displayText(0,0, title, 2);
}


void displayClockTime(int second){
    // display graphic lines around time
    displayColour("white", "black");
    displayLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1);
    displayLine(0, SCREEN_HEIGHT - CHAR_HEIGHT*2-1, SCREEN_WIDTH, SCREEN_HEIGHT - CHAR_HEIGHT*2-1);

    // display the time value
    char time[LINE_SIZE];
    snprintf(time, LINE_SIZE, "%02i-%02i-%02i", clockHour(), clockMinute(), second);
    displayText(SCREEN_WIDTH/2-4*CHAR_WIDTH, SCREEN_HEIGHT-CHAR_HEIGHT*1.5, time, 1);
}


void displayUpdatedTime(){
    // Display time/date when seconds have changed
    int second = clockSecond();
    if (second != prev_sec) {
        prev_sec = second;
        displayClockTime(second);

        // Increment away from keyboard timer
        AFKTimer++;
    }
}


int isUserAFK(char* result){
    if (AFKTimer > 60){
        blankDisplay(result);
        AFKTimer = 0;
        return 1;
    }

    return 0;
}




void runningMainMenu(){
    int running_main_menu= 1;
    int currentOption = 0;

    displayMainMenu();
    printf("Short press the button to navigate, long to confirm\n");

    do
    {
        // check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        // Blanks the display after 1 minute of the button no being pressed
        if(isUserAFK(result)){
            displayMainMenu();
        }

        else if (isLongPressed(result)) {
            running_main_menu = runMainMenuOption(currentOption,result);
            currentOption = 0;
            displayMainMenu();
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result)) {
            // logAdd(GENERAL, "navigate main menu");
            currentOption = navigateMainMenu(currentOption);
        }

    }while(running_main_menu);
}

void displayMainMenu(){
    displayColour("white", "black");
    displayClear();

    displayColour("black","white");
    displayText(0, CHAR_HEIGHT*0, "Feeder Menu", 1);

    displayColour("white","black");
    displayText(0, CHAR_HEIGHT*2, "Setup Time", 1);
    displayText(0, CHAR_HEIGHT*4, "Setup Schedule Time", 1);

    displayExit(0,7);
}

int runMainMenuOption(int currentOption, char* result){
    switch (currentOption){
    case 0:
        runningFeederMenu(result);
        break;

    case 1:
        runningSetupTime(result);
        break;

    case 2:
        printf("Running Schedule Time");
        break;

    case 3:
        return 0;
    }
    return 1;
}


int navigateMainMenu(int currentOption){
    displayColour("white","black");
    switch(currentOption){

    case 0:
        displayText(0, CHAR_HEIGHT*0, "Feeder Menu", 1);
        isOptionSelected(1);
        displayText(0, CHAR_HEIGHT*2, "Setup Time", 1);
        break;

    case 1:
        displayText(0, CHAR_HEIGHT*2, "Setup Time", 1);
        isOptionSelected(1);
        displayText(0, CHAR_HEIGHT*4, "Setup Schedule Time", 1);
        break;

    case 2:
        displayText(0, CHAR_HEIGHT*4, "Setup Schedule Time", 1);
        displayExit(1,7);
        break;

    case 3:
        displayExit(0,7);
        isOptionSelected(1);
        displayText(0, CHAR_HEIGHT*0, "Feeder Menu", 1);
        break;

    default:
    printf("Invalid option\n");
        break;
    }

    return ((currentOption+1)%4);
}


void displayExit(int isSelected, int yAxis) {
    if (isSelected) {
        displayColour("black","white");
    }

    else {
        displayColour("red","black");
    }

    displayText(0, CHAR_HEIGHT*yAxis, "Exit", 1);
}

void isOptionSelected(int isSelected){
    if (isSelected) {
        displayColour("black", "white");
    }

    else{
        displayColour("white", "black");
    }
}


void blankDisplay(char* result){
    int running_blank_display = 1;
    displayColour("white", "grey");
    displayClear();

    do{
        msleep(500L);
        result = buttonState();
        if (isShortPressed(result) || isLongPressed(result)){
            running_blank_display = 0;
        }
    }while(running_blank_display);
}