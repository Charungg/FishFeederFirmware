#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "feeder.h"

#include <string.h>
#include <time.h>

#include "dateTimeStruct.h"
#include "file.h"
#include "menu.h"
#include "userControl.h"


#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80

void runningFeederMenu(feedingScheduleData* feedingScheduleData){
    int running_feeder_menu = 1; // continue flag
    int AFKTimer = 0;
    int prev_second = clockSecond();
    int currentOption = 0;

    textData* feederMenuTexts = setFeederMenuTexts();
    displayFeederMenu(feederMenuTexts,&currentOption);
    printf("Short press the button to navigate, long to confirm\n");

    do{
        // check for the button state every half second
        msleep(500L);
        char* result = buttonState(); // get the button state from the JavaFX application


        // Blanks the display after 1 minute of the button no being pressed
        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displayFeederMenu(feederMenuTexts,&currentOption);
            }
            displayClockTime();
        }

         if (isLongPressed(result, &AFKTimer)) {
            running_feeder_menu = runFeederMenuOption(currentOption);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        if (isShortPressed(result, &AFKTimer)) {
            navigateFeederMenu(feederMenuTexts, &currentOption);
        }

    }while(running_feeder_menu);

    free(feederMenuTexts);
}

textData* setFeederMenuTexts(){
    int n = 5;
    textData* feederMenuTexts = malloc(sizeof(textData) * n);
    feederMenuTexts[0] = setupTextData(0,0,"Feed Now",1,"white","black");
    feederMenuTexts[1] = setupTextData(0,1,"Auto Feed",1,"white","black");
    feederMenuTexts[2] = setupTextData(0,2,"Skip Next Feed",1,"white","black");
    feederMenuTexts[3] = setupTextData(0,3,"Pause",1,"white","black");
    feederMenuTexts[4] = setupTextData(0,5,"Exit",1,"red","black");

    return feederMenuTexts;
}


feedingScheduleData* setFeederScheduleData(){
    feedingScheduleData* feederScheduleData = malloc(sizeof(feedingScheduleData));
    feederScheduleData -> nextFeedTimeIndex = -1;
    feederScheduleData -> totalNumberOfFeed = 0;

    // A = Auto
    // P = Paused
    // S = Skip Next Feed
    feederScheduleData -> operatingMode = 'A';
    feederScheduleData -> amountOfSchedule = 0;
    feederScheduleData -> arrayOfSchedules = NULL;
    return feederScheduleData;
}


void displayFeederMenu(const textData* feederMenuTexts, const int* currentOption){
    displayColour("white","black");
    displayClear();

    for (int i = 0; i < 5; i++){
        displayOptionText(&feederMenuTexts[i],*currentOption == i);
    }


    displayClockTime(clockSecond());
}


int runFeederMenuOption(int currentOption){

    switch(currentOption){
        case 0:
            feedNow();
            break;

        case 1:
            printf("Auto Feed Running\n");
            break;

        case 2:
            printf("Skip Next Feed Running\n");
            break;

        case 3:
            printf("Pause Running\n");
            break;

        case 4:
            return 0;

        default:
            exit(1);
    }

    return 1;
}


void feedNow(){
    // step the motor through a complete rotation (360 degrees)
    for (int i = 0; i < 360; i++) {
        motorStep(); // step the motor in the JavaFX application
        msleep(30L); // can't go too fast, pause some milliseconds
    }

    // numberOfFeeds+=1;
}


void navigateFeederMenu(const textData* feederMenuTexts, int* currentOption){
    int newCurrentOption = ((*currentOption) + 1) % 5;

    displayOptionText(&feederMenuTexts[*currentOption],0);
    displayOptionText(&feederMenuTexts[newCurrentOption],1);

    *currentOption = newCurrentOption;
}


void displayNextFeedTime(textData* startMenuTexts, feedingScheduleData* feedingScheduleData){
    char nextFeedTimeText[] = "Next Feed: ";
    char nextFeedTimeValue[LINE_SIZE];

    int index = feedingScheduleData -> nextFeedTimeIndex;

    if (index != -1){
        int hour = feedingScheduleData -> arrayOfSchedules[index].time -> hour;
        int minute = feedingScheduleData -> arrayOfSchedules[index].time -> minute;
        int second = feedingScheduleData -> arrayOfSchedules[index].time -> second;

        sprintf(nextFeedTimeValue, "%02d:%02d:%02d", hour, minute, second);
        strcat(nextFeedTimeText, nextFeedTimeValue);

        startMenuTexts[1].text = malloc(sizeof(nextFeedTimeText));
        strcpy(startMenuTexts[1].text, nextFeedTimeText);
    }

    else{
        char noNextText[] = "00:00:00";
        startMenuTexts[1].text = malloc(sizeof(noNextText));
        strcat(nextFeedTimeText, noNextText);
        strcpy(startMenuTexts[1].text, nextFeedTimeText);
    }

}

void displayTotalNumberOfFeeds(textData* startMenuTexts, feedingScheduleData* feedingScheduleData){
    char numberOfFeedsText[] = "Number Of Feed: ";
    char text[LINE_SIZE];

    sprintf(text, "%d", feedingScheduleData -> totalNumberOfFeed);
    strcat(numberOfFeedsText, text);

    startMenuTexts[2].text = malloc(sizeof(numberOfFeedsText));
    strcpy(startMenuTexts[2].text, numberOfFeedsText);
}

void displayOperatingMode(textData* startMenuTexts, feedingScheduleData* feedingScheduleData){
    char operatingModeText[] = "Operating Mode: ";

    strncat(operatingModeText, &(feedingScheduleData -> operatingMode), 1);

    startMenuTexts[3].text = malloc(strlen(operatingModeText)+1);
    strcpy(startMenuTexts[3].text, operatingModeText);
}


void checkFeederSchedule(){
    printf("Checking Schedule\n");
    printf("TIME %02d:%02d:%02d\n", clockHour(), clockMinute(), clockSecond());
    // if (globalScheduleArray != NULL){
    //
    //     int sizeOfArray = sizeof(*globalScheduleArray)/sizeof(globalScheduleArray[0]);
    //     printf("SIZE OF SCHEDULING ARRAY = %d\n", sizeOfArray);
    //
    //     for (int i = 0; i < sizeOfArray; i++){
    //         if (isFeedingTime(&globalScheduleArray[0])){
    //
    //             for(int f=0; f<numberOfFeeds; f++){
    //                 feedNow();
    //             }
    //         }
    //     }
    // }
}


int isFeedingTime(feedingSchedule* feederSchedule){
    systemTime* feederTime = feederSchedule->time;
    return (feederSchedule->dayOfWeek == clockDayOfWeek() && feederTime->hour == clockHour() && feederTime -> minute == clockMinute() && feederTime -> second == clockSecond());
}


int isFeederScheduleTheSame(feedingSchedule* feederSchedule, feedingSchedule* otherFeederSchedule){
    return (feederSchedule->dayOfWeek == otherFeederSchedule->dayOfWeek && feederSchedule->time == otherFeederSchedule->time);
}



