#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "fish.h"

void manualFeeder(char* result)
{
    bool waiting_feed_menu = true; // continue flag
    do{

        msleep(500L);

        free(result);
        result = buttonState(); // get the button state from the JavaFX application

        if (strcmp(result, "SHORT_PRESS") == 0){
            waiting_feed_menu = false;
        }

        if (strcmp(result, "LONG_PRESS") == 0){
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


int pauseFeeder()
{

}