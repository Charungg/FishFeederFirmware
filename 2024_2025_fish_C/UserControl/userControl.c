#include <fcntl.h>
#include "userControl.h"

int isLongPressed(const char* result, int* AFKTimer){
    if ((strcmp(result, "LONG_PRESS") == 0)){
        *AFKTimer = 0;
        return 1;
    }
    return 0;
}

int isShortPressed(const char* result, int* AFKTimer){
    if ((strcmp(result, "SHORT_PRESS") == 0)){
        *AFKTimer = 0;
        return 1;
    }
    return 0;
}


