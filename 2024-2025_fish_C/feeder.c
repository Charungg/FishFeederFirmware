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

void runningFeederMenu(char *result){
    int running_feeder_menu = 1; // continue flag
    int prev_sec = 0; // allow detection when seconds value has changed

    int currentOption = 0;

    displayFeederMenu();
    printf("Short press the button to navigate, long to confirm\n");

    do
    {
        // check for the button state every half second
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        // Update time on display
        displayUpdatedTime();

        // Blanks the display after 1 minute of the button no being pressed
        if(isUserAFK(result)){
            displayFeederMenu();
        }

        else if (isLongPressed(result)) {
            running_feeder_menu = runFeederMenuOption(currentOption);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result)) {
            logAdd(GENERAL, "feed menu");
            currentOption = navigateFeederMenu(currentOption);
        }

    }while(running_feeder_menu);
}


void displayFeederMenu(){
    displayColour("white","black");
    displayClear();

    displayColour("black","white");
    displayText(0, CHAR_HEIGHT*0, "Feed Now", 1);

    displayColour("white","black");
    displayText(0, CHAR_HEIGHT*1, "Auto Feed", 1);
    displayText(0, CHAR_HEIGHT*2, "Skip Next Feed", 1);
    displayText(0, CHAR_HEIGHT*3, "Pause", 1);

    displayExit(0,5);
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


void manualFeeder(char* result)
{
    int waiting_feed_menu = 1; // continue flag
    do{

        msleep(500L);

        free(result);
        result = buttonState(); // get the button state from the JavaFX application

        if (isShortPressed(result)){
            waiting_feed_menu = 0;
        }

        if (isLongPressed(result)){
            // step the motor through a complete rotation (360 degrees)
            for (int i = 0; i < 180; i++) {
                motorStep(); // step the motor in the JavaFX application
                msleep(40L); // can't go too fast, pause some milliseconds

                char* result1 = buttonState(); // get the button state from the JavaFX application
                if (strcmp(result1, "SHORT_PRESS") == 0) i = 360;
                free(result1);
            }
        }

    }while(waiting_feed_menu);
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
        displayExit(1,5);
        break;

    case 4:
        displayExit(0,5);
        displayColour("black","white");
        displayText(0, CHAR_HEIGHT*0, "Feed Now", 1);
        break;

    default:
        exit(1);
    }

    return ((currentOption+1)%5);
}


