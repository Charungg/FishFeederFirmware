#include <fcntl.h>
#include "userControl.h"


int isLongPressed(const char* result){
    return (strcmp(result, "LONG_PRESS") == 0);
}

int isShortPressed(const char* result){
    return (strcmp(result, "SHORT_PRESS") == 0);
}

