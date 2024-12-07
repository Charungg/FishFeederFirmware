#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "setupScheduleTime.h"
#include "systemTime.h"
#include "userControl.h"
#include "menu.h"
#include "file.h"

void runningSetupScheduleTime(){
    char* result;
    int running_schedule_time = 1;
    int prev_second = clockSecond();
    int AFKTimer = 0;
    int currentDay = 0;
    int currentOption = 0;

    printf("Short press to navigate, long press to change value\n");
    textData* scheduleTimeTexts = setupScheduleTimeTexts();
    textData* dayTexts = setupDayTexts();
    systemTime* systemTime = getSystemTime();

    displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts, systemTime, &currentOption, &currentDay);
    displaySelected(1);

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                // displaySetupScheduleTime(dateTime, scheduleTimeTexts, &currentOption);
            }
        }


        if (isLongPressed(result, &AFKTimer)){
            // running_schedule_time = selectDateTimeOption(&currentOption, setupScheduleTimeText, dateTime);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result, &AFKTimer)){
            navigateSetupScheduleTime(scheduleTimeTexts, dayTexts, systemTime, &currentOption, &currentDay);
        }

    }while(running_schedule_time);
}


textData* setupDayTexts(){
    const int n = 7;
    textData* dayTexts = malloc(sizeof(textData)*n);
    dayTexts[0] = setupTextData(2,2,"Monday   ",1,"#7B68EE","black");
    dayTexts[1] = setupTextData(2,2,"Tuesday  ",1,"#FF4500","black");
    dayTexts[2] = setupTextData(2,2,"Wednesday",1,"#2E8B57","black");
    dayTexts[3] = setupTextData(2,2,"Thursday ",1,"#98FB98","black");
    dayTexts[4] = setupTextData(2,2,"Friday   ",1,"#DA70D6","black");
    dayTexts[5] = setupTextData(2,2,"Saturday ",1,"#6A5ACD","black");
    dayTexts[6] = setupTextData(2,2,"Sunday   ",1,"#F5FFFA","black");
    return dayTexts;

}


textData* setupScheduleTimeTexts(){
    const int n = 3;
    textData* textDataPtr = malloc(sizeof(textData)*n);
    textDataPtr[0] = setupTextData(0,7,"Exit", 1,"red","black");
    textDataPtr[1] = setupTextData(7,7,"Remove", 1,"#008080","black");
    textDataPtr[2] = setupTextData(17,7,"Save", 1,"#7CFC00","black");
    return textDataPtr;
}


void displaySetupScheduleTimeTexts(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay){
    displayColour("white","black");
    displayClear();

    displayTimeBox();
    displayOptionText(&dayTexts[*currentDay], *currentOption == 0);
    displayHour(systemTime, *currentOption == 1);
    displayMinute(systemTime,*currentOption == 2);
    displaySecond(systemTime,*currentOption == 3);

    displayOptionText(&scheduleTimeTexts[0],*currentOption == 4);
    displayOptionText(&scheduleTimeTexts[1],*currentOption == 5);
    displayOptionText(&scheduleTimeTexts[2],*currentOption == 6);
}


void navigateSetupScheduleTime(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay){
    switch(*currentOption){
        case 0:
            displayOptionText(&dayTexts[*currentDay], 0);
            displayHour(systemTime, 1);
            break;

        case 1:
            displayHour(systemTime, 0);
            displayMinute(systemTime,1);
            break;

        case 2:
            displayMinute(systemTime,0);
            displaySecond(systemTime,1);
            break;

        case 3:
            displaySecond(systemTime,0);
            displayOptionText(&scheduleTimeTexts[0],1);
            break;

        case 4:
            displayOptionText(&scheduleTimeTexts[0],0);
            displayOptionText(&scheduleTimeTexts[1],1);
            break;

        case 5:
            displayOptionText(&scheduleTimeTexts[1],0);
            displayOptionText(&scheduleTimeTexts[2],1);
            break;

        case 6:
            displayOptionText(&scheduleTimeTexts[2],0);
            displayOptionText(&dayTexts[*currentDay], 1);
            break;

        default:
            exit(1);
    }

    *currentOption = ((*currentOption + 1)% 7);
}



