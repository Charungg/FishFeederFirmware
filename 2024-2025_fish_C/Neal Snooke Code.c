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


// void runningMainMenu(char* title){
//
//     // the main loop that checks for button presses, enters the feed 'menu'
//     // and allows the user to long press the button to initiate a feed
//     // TODO This function should be split into smaller pieces - factor out feed menu.
//
//     // display console message
//     printf("Long press the button to quit, short press to enter menu\n");
//
//     int running_main_menu = 1; // continue flag
//     int prev_sec = 0; // allow detection when seconds value has changed
//     while (running_main_menu) {
//
//         // display time/date when seconds have changed
//         int second = clockSecond();
//         if (second != prev_sec) {
//             prev_sec = second;
//             displayTime(second);
//         }
//
//         // check for the button state every half second
//         msleep(500L);
//         char *result = buttonState(); // get the button state from the JavaFX application
//
//         if (strcmp(result, "LONG_PRESS") == 0) {
//             running_main_menu = false;
//         }
//
//         // check the result and quit this look if the button is short pressed
//         if (strcmp(result, "SHORT_PRESS") == 0) {
//
//             logAdd(GENERAL, "feed menu");
//             printf("Long press the button to feed, short press to exit\n");
//             //running_main_menu = 0; // exit the loop
//
//             // displayNavMenu(title);
//             // runningNavigationMenu(title);
//             // manualFeeder(result);
//
//             free(result);
//
//             // back to main screen
//             displayClear(); // clear JavaFX display.
//             displayText(0,0, title, 2);
//         }
//     }
// }


// void manualFeeder(char* result)
// {
//     bool waiting_feed_menu = true; // continue flag
//     while (waiting_feed_menu){
//
//         msleep(500L);
//
//         free(result);
//         result = buttonState(); // get the button state from the JavaFX application
//
//         if (strcmp(result, "SHORT_PRESS") == 0){
//             waiting_feed_menu = false;
//         }
//
//         if (strcmp(result, "LONG_PRESS") == 0){
//             // step the motor through a complete rotation (360 degrees)
//             for (int i = 0; i < 360; i++) {
//                 motorStep(); // step the motor in the JavaFX application
//                 msleep(40L); // can't go too fast, pause some milliseconds
//
//                 char* result1 = buttonState(); // get the button state from the JavaFX application
//                 if (strcmp(result1, "SHORT_PRESS") == 0) i = 360;
//                 free(result1);
//             }
//         }
//     }
// }