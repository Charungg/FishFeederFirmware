#include <fcntl.h>
#include "userControl.h"


int isLongPressed(char* result){
    return (strcmp(result, "LONG_PRESS") == 0);
}

int isShortPressed(char* result){
    return (strcmp(result, "SHORT_PRESS") == 0);
}

