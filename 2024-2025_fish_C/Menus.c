#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fish.h"


#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80 // size of the line buffer

void displayStartMenu(char* title){
    // output banner (see fish.h for details)
    displayColour("white", "black"); // white text on black background
    displayClear(); // clear JavaFX display.
    displayText(0,0, title, 2);
}

void displayTime(int second)
{
    // display graphic lines around time
    displayColour("white", "black");
    displayLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1);
    displayLine(0, SCREEN_HEIGHT - CHAR_HEIGHT*2-1, SCREEN_WIDTH, SCREEN_HEIGHT - CHAR_HEIGHT*2-1);

    // display the time value
    char time[LINE_SIZE];
    snprintf(time, LINE_SIZE, "%02i-%02i-%02i", clockHour(), clockMinute(), second);
    displayText(SCREEN_WIDTH/2-4*CHAR_WIDTH, SCREEN_HEIGHT-CHAR_HEIGHT*1.5, time, 1);
}


void runningStartMenu(char* title){
    int running_start_menu = 1; // continue flag
    int prev_sec = 0; // allow detection when seconds value has changed

    displayStartMenu(title);

    printf("Short press to enter main menu, Long press the button to quit\n");

    do{
        // display time/date when seconds have changed
        int second = clockSecond();
        if (second != prev_sec) {
            prev_sec = second;
            displayTime(second);
        }
        // check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        if (strcmp(result, "LONG_PRESS") == 0) {
            running_start_menu = 0;
        }

        // check the result and quit this look if the button is short pressed
        if (strcmp(result, "SHORT_PRESS") == 0)
        {
            logAdd(GENERAL, "main menu");

            displayMainMenu();
            runningMainMenu();
            displayStartMenu(title);
            free(result);
        }

    }while(running_start_menu);
}






// Main menu functions
void displayMainMenu(){
    displayColour("white", "black");
    displayClear();

    displayColour("black","white");
    displayText(0, CHAR_HEIGHT*0, "Feeder Menu", 1);

    displayColour("white","black");
    displayText(0, CHAR_HEIGHT*2, "Setup Time", 1);
    displayText(0, CHAR_HEIGHT*4, "Setup Schedule Time", 1);

    displayColour("red","black");
    displayText(0, CHAR_HEIGHT*7, "Exit", 1);
}

void runningMainMenu(){
    int running_main_menu= 1; // continue flag

    int currentOption = 0;

    displayMainMenu();
    printf("Short press the button to navigate, long to confirm\n");

    do
    {
        // check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        if (strcmp(result, "LONG_PRESS") == 0) {
            running_main_menu = runMainMenuOption(currentOption,result);
            currentOption = 0;
            displayMainMenu();
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        if (strcmp(result, "SHORT_PRESS") == 0) {
            // logAdd(GENERAL, "navigate main menu");
            currentOption = navigateMainMenu(currentOption);
        }

    }while(running_main_menu);
}

int navigateMainMenu(int currentOption)
{
    displayColour("white","black");
    switch(currentOption){

    case 0:
        displayText(0, CHAR_HEIGHT*0, "Feeder Menu", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*2, "Setup Time", 1);
        break;

    case 1:
        displayText(0, CHAR_HEIGHT*2, "Setup Time", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*4, "Setup Schedule Time", 1);
        break;

    case 2:
        displayText(0, CHAR_HEIGHT*4, "Setup Schedule Time", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*7, "Exit", 1);
        break;

    case 3:
        displayColour("red","black");
        displayText(0, CHAR_HEIGHT*7, "Exit", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*0, "Feeder Menu", 1);
        break;

    default:
    printf("Invalid option\n");
        break;
    }

    return ((currentOption+1)%4);
}


int runMainMenuOption(int currentOption, char* result)
{
    switch(currentOption){
        case 0:
        displayFeederMenu();
        runningFeederMenu(result);

            break;

        case 1:
            printf("Setup Time Running\n");
            break;

        case 2:
            printf("Setup Schedule Time Running\n");
            break;

        case 3:
            return 0;
    }

    return 1;
}


// Feeder menu functions
void displayFeederMenu(){
    displayClear();

    displayColour("black","white");
    displayText(0, CHAR_HEIGHT*0, "Feed Now", 1);

    displayColour("white","black");
    displayText(0, CHAR_HEIGHT*1, "Auto Feed", 1);
    displayText(0, CHAR_HEIGHT*2, "Skip Next Feed", 1);
    displayText(0, CHAR_HEIGHT*3, "Pause", 1);

    displayColour("red","black");
    displayText(0, CHAR_HEIGHT*5, "Exit", 1);
}

void runningFeederMenu(char *result){
    int running_feeder_menu = 1; // continue flag
    int prev_sec = 0; // allow detection when seconds value has changed

    int currentOption = 0;

    displayFeederMenu();
    printf("Short press the button to navigate, long to confirm\n");

    do
    {
        // display time/date when seconds have changed
        int second = clockSecond();
        if (second != prev_sec) {
            prev_sec = second;
            displayTime(second);
        }

        // check for the button state every half second
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if (strcmp(result, "LONG_PRESS") == 0) {
            running_feeder_menu = runFeederMenuOption(currentOption);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        if (strcmp(result, "SHORT_PRESS") == 0) {
            logAdd(GENERAL, "feed menu");
            currentOption = navigateFeederMenu(currentOption);
        }

    }while(running_feeder_menu);

    displayClear();
}


int navigateFeederMenu(int currentOption){
    displayColour("white","black");
    switch(currentOption)
    {
    case 0:
        displayText(0, CHAR_HEIGHT*0, "Feed Now", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*1, "Auto Feed", 1);
        break;

    case 1:
        displayText(0, CHAR_HEIGHT*1, "Auto Feed", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*2, "Skip Next Feed", 1);
        break;

    case 2:
        displayText(0, CHAR_HEIGHT*2, "Skip Next Feed", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*3, "Pause", 1);
        break;

    case 3:
        displayText(0, CHAR_HEIGHT*3, "Pause", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*5, "Exit", 1);
        break;

    case 4:
        displayColour("red","black");
        displayText(0, CHAR_HEIGHT*5, "Exit", 1);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*0, "Feed Now", 1);
        break;
    }
    return ((currentOption+1)%5);
}


int runFeederMenuOption(int currentOption){

    switch(currentOption){
        case 0:
            printf("Feed Now Running\n");
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
            printf("Invalid option\n");
            return 0;
    }

    return 1;
}
