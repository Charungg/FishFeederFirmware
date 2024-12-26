//
// Created by Neal Snooke on 12/07/2024.
// Version 1.1
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "fish.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80 // size of the line buffer

/**
 * the function that is the entry point for the fish feeder C program main logic
 * it is called by jniSetup() from main, once the GUI thread has been initialised.
 *
 * this is a test implementation that demonstrates the clock, button and display
 * It should be split into smaller functions.
 */
void userProcessing() {
    char title[12] = "ArduFeed";

    // add a message to the log METHOD_ENTRY category (see fish.h for details)
    logAdd(METHOD_ENTRY, "userProcessing()");

    // display an information message on the GUI debug log display area (see fish.h)
    infoMessage("Welcome to the fish feeder test program");

    // set the real time clock to 1400:00 on the 25th May 1968
    clockSet(0, 0, 14, 25, 05, 1968); // set the time to 12:30:00
    printf("Clock thinks the year is %d\n", clockYear());
    printf("Clock thinks the day is %d\n", clockDayOfWeek());
    printf("Clock %d:%d:%d %d/%d/%d\n", clockHour(), clockMinute(), clockSecond(),  clockYear(), clockMonth(), clockDay());

    foodFill(50); // fill the food container Note: maximum 50%

    // output banner (see fish.h for details)
    displayColour("white", "black"); // white text on black background
    displayClear(); // clear JavaFX display.
    displayText(0,0, title, 2);

    // the main loop that checks for button presses, enters the feed 'menu'
    // and allows the user to long press the button to initiate a feed
    // TODO This function should be split into smaller pieces - e.g. factor out feed menu.

    // display console message
    printf("Long press the button to quit, short press to enter menu\n");

    int running_main_menu = 1; // continue flag
    int prev_sec = 0; // allow detection when seconds value has changed
    while (running_main_menu) {

        // display time/date when seconds have changed
        int second = clockSecond();
        if (second != prev_sec) {
            prev_sec = second;

            // display graphic lines around time
            displayColour("white", "black");
            displayLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1);
            displayLine(0, SCREEN_HEIGHT - CHAR_HEIGHT*2-1, SCREEN_WIDTH, SCREEN_HEIGHT - CHAR_HEIGHT*2-1);

            // display the time value
            char time[LINE_SIZE];
            snprintf(time, LINE_SIZE, "%02i-%02i-%02i", clockHour(), clockMinute(), second);
            displayText(SCREEN_WIDTH/2-4*CHAR_WIDTH, SCREEN_HEIGHT-CHAR_HEIGHT*1.5, time, 1);
        }

        // check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        if (strcmp(result, "LONG_PRESS") == 0) {
            running_main_menu = false;
        }

        // check the result and quit this look if the button is short pressed
        if (strcmp(result, "SHORT_PRESS") == 0) {

            logAdd(GENERAL, "feed menu");
            printf("Long press the button to feed, short press to exit\n");
            //running_main_menu = 0; // exit the loop

            displayClear();
            displayText(0,0, title, 2);

            displayColour( "black", "white");
            displayText(0,CHAR_HEIGHT*4, "Manual feed", 1);
            displayColour("white", "black"); // JavaFX display. white text on black background
            displayText(0,CHAR_HEIGHT*7, "Exit", 1);

            bool waiting_feed_menu = true; // continue flag
            while (waiting_feed_menu){

                msleep(500L);

                free(result);
                result = buttonState(); // get the button state from the JavaFX application

                if (strcmp(result, "SHORT_PRESS") == 0){
                    waiting_feed_menu = false;
                }

                if (strcmp(result, "LONG_PRESS") == 0){
                    // step the motor through a complete rotation (360 degrees)
                    for (int i = 0; i < 360; i++) {
                        motorStep(); // step the motor in the JavaFX application
                        msleep(40L); // can't go too fast, pause some milliseconds

                        char* result1 = buttonState(); // get the button state from the JavaFX application
                        if (strcmp(result1, "SHORT_PRESS") == 0) i = 360;
                        free(result1);
                    }
                }
            }

            free(result);

            // back to main screen
            displayClear(); // clear JavaFX display.
            displayText(0,0, title, 2);
        }
    }

    logAdd(METHOD_ENTRY, "userProcessing() done");
}

/**
 * main entry point for the Fish Feeder Emulator program
 * @return
 */
int main() {
    // turn on all console logging for the Java and fish feeder JNI C code. Shouldn't need this
    //logAddInfo(GENERAL | METHOD_ENTRY | JNI_MESSAGES | JFX_MESSAGES | THREAD_NAME | THREAD_ID | STACK_INFO);
    //logAddInfo( GENERAL | JFX_MESSAGES | THREAD_NAME | THREAD_ID );

    //logRemoveInfo(METHOD_ENTRY); // don't show these debug messages in the gui

    // add the GUI debug window showing messages received
    logAddInfo( GUI_INFO_DEBUG);

    // add a log entry for entry to this method
    logAdd(METHOD_ENTRY, "main(). test of Fish Feed Hardware Emulator using a JavaFX GUI and jni");

    // start the JVM and set up the JNI environment
    // this will result in the userProcessing() function being called to run the C part of the program
    if (jniSetup() != 0) return 1;

    // hand over this thread to Java FX to create the fish hardware emulator GUI.
    // FX will terminate the application when the main window is closed
    // or the C application calls the FishFeederEmulator.exit() function.
    javaFx();

    return(EXIT_SUCCESS);
}