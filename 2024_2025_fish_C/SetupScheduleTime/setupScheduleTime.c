#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "setupScheduleTime.h"

#include <string.h>

#include "systemTime.h"
#include "userControl.h"
#include "menu.h"
#include "file.h"
#include "setupTime.h"

#define LINE_SIZE 80

void runningSetupScheduleTime(){
    char* result;
    int running_schedule_time = 1;
    int prev_second = clockSecond();
    int AFKTimer = 0;
    int currentDay = 0;
    int currentOption = 0;

    printf("Short press to navigate, long press to change value\n");

    textData* dayTexts = setupDayTexts();
    textData* scheduleTimeTexts = setupScheduleTimeTexts();
    systemDateTime* systemDateTime = getSystemDateTime();

    displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts, systemDateTime -> time, &currentOption, &currentDay);

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                // displaySetupScheduleTime(dateTime, scheduleTimeTexts, &currentOption);
            }
        }


        if (isLongPressed(result, &AFKTimer)){
            running_schedule_time = selectSetupScheduleTimeOption(scheduleTimeTexts, dayTexts, systemDateTime, &currentOption, &currentDay);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result, &AFKTimer)){
            navigateSetupScheduleTime(scheduleTimeTexts, dayTexts, systemDateTime -> time, &currentOption, &currentDay);
        }

    }while(running_schedule_time);

    // printf("SystemTimeSaveExist: %d\n",isSystemTimeSaveExist());
    // printf("ScheduleTimeSaveExist: %d\n",isScheduleTimeSaveExist());
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
    const int n = 4;
    textData* textDataPtr = malloc(sizeof(textData)*n);
    textDataPtr[0] = setupTextData(14,2,"99", 1,"#00FFFF","black");
    textDataPtr[1] = setupTextData(0,7,"Exit", 1,"red","black");
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


int selectSetupScheduleTimeOption(textData* scheduleTimeTexts, textData* dayTexts, systemDateTime* dateTime, int* currentOption, int* currentDay){
    void (*incrementFunction)(systemDateTime*);

    switch(*currentOption){
        case 0:
            runningModifyDayText(scheduleTimeTexts, dayTexts, dateTime ->  time, currentOption, currentDay);
        break;

        case 1:
            runningModifyNumberOfFeedText(scheduleTimeTexts, dayTexts, dateTime ->  time, currentOption, currentDay);
            break;

        case 2:
            incrementFunction = &incrementHour;
            runningModifyDateTimeValue(incrementFunction, scheduleTimeTexts, dateTime, currentOption);
            break;

        case 3:
            incrementFunction = &incrementMinute;
            runningModifyDateTimeValue(incrementFunction, scheduleTimeTexts, dateTime, currentOption);
            break;
        case 4:
            incrementFunction = &incrementSecond;
            runningModifyDateTimeValue(incrementFunction, scheduleTimeTexts, dateTime, currentOption);
            break;

        case 5:
            return 0;

        case 6:
            printf("SaveScheduleTime()");
            saveScheduleTime(dateTime->time, currentOption, currentDay);
            break;

        default:
            exit(1);
    }

    return 1;
}


void runningModifyDayText(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay){
    char* result;
    int running_modify_day_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,systemTime,currentOption,currentDay);
                displaySelected(1);
            }
        }


        if (isLongPressed(result, &AFKTimer)){
            running_modify_day_menu = 0;
            displaySelected(0);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result, &AFKTimer)){
            *currentDay = ((*currentDay + 1) % 7);
            displayOptionText(&dayTexts[*currentDay], 1);
        }
    }while(running_modify_day_menu);

}


void runningModifyNumberOfFeedText(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay){
    char* result;
    int running_modify_number_of_feed_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,systemTime,currentOption,currentDay);
                displaySelected(1);
            }
        }

        if (isLongPressed(result, &AFKTimer)){
            running_modify_number_of_feed_menu = 0;
            displaySelected(0);
            free(result);
        }

        else if (isShortPressed(result, &AFKTimer)){
            printf("UPDATE NUMBER OF FEED\n");
            updateNumberOfFeed(scheduleTimeTexts);
        }
    }while(running_modify_number_of_feed_menu);
}


void updateNumberOfFeed(textData* scheduleTimeTexts){
    char newNumberOfFeedText[LINE_SIZE];
    char* numberOfFeedText = scheduleTimeTexts[0].text;
    int n;
    int numberOfFeed;

    // stspn finds the index position which ignores leading zero within the numberOfFeedText.
    n = strspn(numberOfFeedText,"0");

    // Convert string numberOfFeed to an int
    numberOfFeed = atoi(&numberOfFeedText[n]);

    // numberOfFeed int version will be incremented by 1
    numberOfFeed = ((numberOfFeed + 1) % 100);

    sprintf(newNumberOfFeedText,"%02d",numberOfFeed);
    scheduleTimeTexts[0].text = malloc(sizeof(newNumberOfFeedText));
    strcpy(scheduleTimeTexts[0].text, newNumberOfFeedText);

    displayOptionText(&scheduleTimeTexts[0], 1);
}


void navigateSetupScheduleTime(textData* scheduleTimeTexts, textData* dayTexts, systemTime* systemTime, int* currentOption, int* currentDay){
    switch(*currentOption){
        case 0:
            displayOptionText(&dayTexts[*currentDay], 0);
            displayOptionText(&scheduleTimeTexts[0],1);
            break;

        case 1:
            displayOptionText(&scheduleTimeTexts[0],0);
            displayHour(systemTime, 1);
            break;

        case 2:
            displayHour(systemTime, 0);
            displayMinute(systemTime,1);
            break;

        case 3:
            displayMinute(systemTime,0);
            displaySecond(systemTime,1);
            break;

        case 4:
            displaySecond(systemTime,0);
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



