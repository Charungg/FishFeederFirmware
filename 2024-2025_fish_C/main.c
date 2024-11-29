//
// Created by Neal Snooke on 12/07/2024.
// Version 1.1
//
#include <stdio.h>
#include <stdlib.h>
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
    char * title = "ArduFEED";
    introduction();
    // runningStartMenu(title);
    test();

    logAdd(METHOD_ENTRY, "userProcessing() done");
}

void test()
{
    char* result;

    manualFeeder(result);
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

//----------------------------DO NOT TOUCH ABOVE-------------------------------

void introduction(void){
    // add a message to the log METHOD_ENTRY category (see fish.h for details)
    logAdd(METHOD_ENTRY, "userProcessing()");

    // display an information message on the GUI debug log display area (see fish.h)
    infoMessage("Welcome to the fish feeder test program");

    // set the real time clock to 13:00:00 on the 25th May 1968
    clockSet(0, 0, 14, 25, 05, 1968); // set the time to 12:30:00
    printf("Clock thinks the year is %d\n", clockYear());
    printf("Clock thinks the day is %d\n", clockDayOfWeek());
    printf("Clock %d:%d:%d %d/%d/%d\n", clockHour(), clockMinute(), clockSecond(),  clockYear(), clockMonth(), clockDay());


    foodFill(50); // fill the food container Note: maximum 50%
}












