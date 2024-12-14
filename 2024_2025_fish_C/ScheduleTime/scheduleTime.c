#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "scheduleTime.h"

#include "dateTime.h"
#include "feeder.h"
#include "userControl.h"
#include "menu.h"
#include "feederSchedule.h"
#include "feedingScheduleStruct.h"
#include "file.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define LINE_SIZE 80

void runningSetupScheduleTime(feederSystem* feedingScheduleData){
    char* result;
    int running_schedule_time = 1;
    int prev_second = clockSecond();
    int AFKTimer = 0;
    int currentOption = 0;

    printf("Short press to navigate, long press to change value\n");

    textData* dayTexts = setupDayTexts();
    textData* scheduleTimeTexts = setupScheduleTimeTexts();
    feedingSchedule* feedingSchedule = setupFeedingSchedule();

    displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts, feedingSchedule, &currentOption);

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts, feedingSchedule, &currentOption);
            }
        }

        if (isLongPressed(result, &AFKTimer)){
            running_schedule_time = selectSetupScheduleTimeOption(scheduleTimeTexts, dayTexts, feedingScheduleData, feedingSchedule, &currentOption);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result, &AFKTimer)){
            navigateSetupScheduleTime(scheduleTimeTexts, dayTexts, feedingSchedule, &currentOption);
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
    const int n = 4;
    textData* scheduleTimeTexts = malloc(sizeof(textData)*n);
    scheduleTimeTexts[0] = setupTextData(0,7,"Exit", 1,"red","black");
    scheduleTimeTexts[1] = setupTextData(17,7,"Save", 1,"#7CFC00","black");
    return scheduleTimeTexts;
}


feedingSchedule* setupFeedingSchedule(){
    feedingSchedule* feedingSchedule = malloc(sizeof(feedingSchedule));
    feedingSchedule -> dayOfWeek = 0;
    feedingSchedule -> numberOfFeed = 0;
    feedingSchedule -> time = *getSystemTime();
    return feedingSchedule;
}


void displaySetupScheduleTimeTexts(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* feedingSchedule, const int* currentOption){
    displayColour("white","black");
    displayClear();

    displayTimeBox();
    displayOptionText(&dayTexts[feedingSchedule->dayOfWeek], *currentOption == 0);
    displayNumberOfFeedText(feedingSchedule, *currentOption == 1);


    displayHour(&feedingSchedule -> time, *currentOption == 2);
    displayMinute(&feedingSchedule -> time,*currentOption == 3);
    displaySecond(&feedingSchedule -> time,*currentOption == 4);

    displayOptionText(&scheduleTimeTexts[0],*currentOption == 5);
    displayOptionText(&scheduleTimeTexts[1],*currentOption == 6);
}


void displayNumberOfFeedText(const feedingSchedule* feedingSchedule, int isSelected){
    char numberOfFeed[LINE_SIZE];

    if (isSelected){
        displayColour("black","white");
    }

    else{
        displayColour("white","#5F9EA0");
    }

    sprintf(numberOfFeed, "%02d",feedingSchedule -> numberOfFeed);
    displayText(CHAR_WIDTH*14,CHAR_HEIGHT*2,numberOfFeed,1);
}


int selectSetupScheduleTimeOption(textData* scheduleTimeTexts, textData* dayTexts, feederSystem* feedingScheduleData, feedingSchedule* feedingSchedule, int* currentOption){
    void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime);

    switch(*currentOption){
        case 0:
            runningModifyDayOfTheWeekText(scheduleTimeTexts, dayTexts, feedingSchedule, currentOption);
        break;

        case 1:
            runningModifyNumberOfFeedText(scheduleTimeTexts, dayTexts, feedingSchedule, currentOption);
            break;

        case 2:
            incrementScheduleTimeFunctionPtr = &incrementScheduleHour;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, feedingSchedule, currentOption);
            break;

        case 3:
            incrementScheduleTimeFunctionPtr = &incrementScheduleMinute;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, feedingSchedule, currentOption);
            break;
        case 4:
            incrementScheduleTimeFunctionPtr = &incrementScheduleSecond;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, feedingSchedule, currentOption);
            break;

        case 5:
            return 0;

        case 6:
            addSchedule(feedingScheduleData, feedingSchedule);
            displayOptionText(&scheduleTimeTexts[1],0);
            msleep(500);
            displayOptionText(&scheduleTimeTexts[1],1);
            break;

        default:
            exit(1);
    }

    return 1;
}


void runningModifyDayOfTheWeekText(const textData* scheduleTimeTexts, textData* dayTexts, feedingSchedule* scheduleData, const int* currentOption){
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
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,scheduleData,currentOption);
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
            updateDayOfTheWeek(scheduleData,dayTexts);
        }
    }while(running_modify_day_menu);
}


void updateDayOfTheWeek(feedingSchedule* scheduleData, textData* dayTexts){
    (scheduleData -> dayOfWeek) = ((scheduleData->dayOfWeek + 1) % 7);
    displayOptionText(&dayTexts[scheduleData -> dayOfWeek], 1);
}



void runningModifyNumberOfFeedText(const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* feedingSchedule, const int* currentOption){
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
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,feedingSchedule,currentOption);
                displaySelected(1);
            }
        }

        if (isLongPressed(result, &AFKTimer)){
            running_modify_number_of_feed_menu = 0;
            displaySelected(0);
            free(result);
        }

        else if (isShortPressed(result, &AFKTimer)){
            updateNumberOfFeed(feedingSchedule);
        }
    }while(running_modify_number_of_feed_menu);
}


void updateNumberOfFeed(feedingSchedule* feedingSchedule){
    int *numberOfFeedPtr = &feedingSchedule -> numberOfFeed;
    *numberOfFeedPtr = (*numberOfFeedPtr + 1) % 100;

    displayNumberOfFeedText(feedingSchedule,1);
}


void runningModifyScheduleTimeText(void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime), textData* scheduleTimeTexts, textData* dayTexts, feedingSchedule* feedingSchedule, int* currentOption){
    char* result;
    int running_modify_schedule_time_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,feedingSchedule,currentOption);
                displaySelected(1);
            }
        }

        if (isLongPressed(result, &AFKTimer)){
            running_modify_schedule_time_menu = 0;
            displaySelected(0);
            free(result);
        }

        else if (isShortPressed(result, &AFKTimer)){
            incrementScheduleTimeFunctionPtr(&feedingSchedule->time);
        }
    }while(running_modify_schedule_time_menu);
}


void incrementScheduleHour(systemTime* systemTime){
    int* scheduleSecondHour = &systemTime -> hour;
    *scheduleSecondHour = (*scheduleSecondHour + 1) % 24;

    displayHour(systemTime,1);
}


void incrementScheduleMinute(systemTime* systemTime){
    int* scheduleSecondMinute = &systemTime -> minute;
    *scheduleSecondMinute = (*scheduleSecondMinute + 1) % 60;

    displayMinute(systemTime,1);
}


void incrementScheduleSecond(systemTime* systemTime){
    int* scheduleSecond = &systemTime -> second;
    *scheduleSecond =  (*scheduleSecond + 1) % 60;

    displaySecond(systemTime,1);
}


void navigateSetupScheduleTime(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* feedingSchedule, int* currentOption){
    switch(*currentOption){
        case 0:
            displayOptionText(&dayTexts[feedingSchedule->dayOfWeek], 0);
            displayNumberOfFeedText(feedingSchedule, 1);
            break;

        case 1:
            displayNumberOfFeedText(feedingSchedule, 0);
            displayHour(&feedingSchedule -> time, 1);
            break;

        case 2:
            displayHour(&feedingSchedule -> time, 0);
            displayMinute(&feedingSchedule -> time,1);
            break;

        case 3:
            displayMinute(&feedingSchedule -> time,0);
            displaySecond(&feedingSchedule-> time,1);
            break;

        case 4:
            displaySecond(&feedingSchedule -> time,0);
            displayOptionText(&scheduleTimeTexts[0],1);
            break;

        case 5:
            displayOptionText(&scheduleTimeTexts[0],0);
            displayOptionText(&scheduleTimeTexts[1],1);
            break;

        case 6:
            displayOptionText(&scheduleTimeTexts[1],0);
            displayOptionText(&dayTexts[feedingSchedule->dayOfWeek], 1);
            break;


        default:
            exit(1);
    }

    *currentOption = ((*currentOption + 1)% 7);
}





