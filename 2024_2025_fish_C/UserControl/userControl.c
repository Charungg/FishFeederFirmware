#include <string.h>
#include "userControl.h"

/**
 * Checks if the button has been long pressed.
 * @param result keeps track of the button state in the JavaFX GUI.
 * @param AFKTimer keep track when the user has been AFK for x seconds.
 * @return 1 if button has been long pressed, otherwise 0.
 */
int isLongPressed(const char* result, int* AFKTimer){
    // Checks if the button state from GUI is long pressed.
    if ((strcmp(result, "LONG_PRESS") == 0)){
        // Reset the AFK Timer to 0.
        *AFKTimer = 0;
        return 1;
    }

    // Long pressed did not happen.
    return 0;
}


/**
 * Checks if the button has been short pressed.
 * @param result keeps track of the button state in the JavaFX GUI.
 * @param AFKTimer keep track when the user has been AFK for x seconds.
 * @return 1 if button has been short pressed, otherwise 0.
 */
int isShortPressed(const char* result, int* AFKTimer){
    // Checks if the button state from GUI is short pressed.
    if ((strcmp(result, "SHORT_PRESS") == 0)){
        *AFKTimer = 0;
        // Reset the AFK Timer to 0.
        return 1;
    }

    // Short pressed did not happen.
    return 0;
}


