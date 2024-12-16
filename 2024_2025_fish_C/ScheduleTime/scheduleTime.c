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

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define LINE_SIZE 80


/**
 * Displays and runs the setup schedule time.
 * Allows the user to set their own feeding schedule on specific day of the week and time.
 * This is where the user goes to create feeding schedules for the fishes.
 * @param feeder is the feeder system used to ensure the fishes are fed when feeding schedules activates.
 */
void runningSetupScheduleTimeMenu(feederSystem* feeder){
    char* result;
    int running_schedule_time = 1;
    int prev_second = clockSecond();
    int AFKTimer = 0;
    int currentOption = 0;

    // Set up schedule time menu texts.
    textData* dayTexts = setupDayTexts();
    textData* scheduleTimeTexts = setupScheduleTimeTexts();
    // Create a default feeding schedule for the user to modify and save to the feeder system.
    feedingSchedule* feedingSchedule = setupFeedingSchedule();

    // Displays all the schedule time menu texts.
    displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts, feedingSchedule, &currentOption);

    printf("Short press to navigate, long press to change value\n");

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        // Checks if a second has past.
        if(isTimeUpdated(&prev_second)){
            // Blanks the display after 1 minute of the button no being pressed
            // And also checks if its feeding time from the feeder system.
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts, feedingSchedule, &currentOption);
            }
        }

        // Checks if button has been long pressed.
        if (isLongPressed(result, &AFKTimer)){
            // Runs the user currently selected texts operations.
            // Allows to change day of week, time, exit or save.
            running_schedule_time = selectSetupScheduleTimeOption(scheduleTimeTexts, dayTexts, feeder, feedingSchedule, &currentOption);
            free(result);
        }

        // Checks if button has been short pressed.
        else if (isShortPressed(result, &AFKTimer)){
            // Allow the user to navigate between schedule time menu texts.
            navigateSetupScheduleTime(scheduleTimeTexts, dayTexts, feedingSchedule, &currentOption);
        }

    }while(running_schedule_time);
}


/**
 * Set up the day texts for the schedule time menu.
 * @return an array of day texts from Monday - Sunday.
 */
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


/**
 * Set up schedule time texts like Exit and Save.
 * @return an array of schedule time texts for the schedule time menu.
 */
textData* setupScheduleTimeTexts(){
    const int n = 4;
    textData* scheduleTimeTexts = malloc(sizeof(textData)*n);
    scheduleTimeTexts[0] = setupTextData(0,7,"Exit", 1,"red","black");
    scheduleTimeTexts[1] = setupTextData(17,7,"Save", 1,"#7CFC00","black");
    return scheduleTimeTexts;
}

/**
 * Set up a default feeder schedule for the user to modify to create their very own feeder schedules for the fishes.
 * @return
 */
feedingSchedule* setupFeedingSchedule(){
    feedingSchedule* feedingSchedule = malloc(sizeof(feedingSchedule));
    feedingSchedule -> dayOfWeek = 0;
    feedingSchedule -> numberOfFeed = 0;
    // Get the current system time for the user to modify for the feeding schedule.
    feedingSchedule -> time = *getSystemTime();
    return feedingSchedule;
}


/**
 * Display all the schedule time menu texts on the GUI
 * Display the day of the week, number of feed and time from the feeding schedule.
 * @param scheduleTimeTexts contains options text such as Save and Exit.
 * @param dayTexts contains the Monday - Sunday text for the GUI
 * @param feedingSchedule contains feeding time and number of feed.
 * @param currentOption specify what the user is currently selecting on the schedule time menu texts.
 */
void displaySetupScheduleTimeTexts(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* feedingSchedule, const int* currentOption){
    displayColour("white","black");
    displayClear();

    // Display the feeding schedule number of feed and day of the week.
    displayOptionText(&dayTexts[feedingSchedule->dayOfWeek], *currentOption == 0);
    displayNumberOfFeedText(feedingSchedule, *currentOption == 1);

    // Display the time box around.
    displayTimeBox();
    displayHour(&feedingSchedule -> time, *currentOption == 2);
    displayMinute(&feedingSchedule -> time,*currentOption == 3);
    displaySecond(&feedingSchedule -> time,*currentOption == 4);

    // Display Save and Exit text on the bottom of the GUI.
    displayOptionText(&scheduleTimeTexts[0],*currentOption == 5);
    displayOptionText(&scheduleTimeTexts[1],*currentOption == 6);
}


/**
 * Display the number of feed on the schedule time menu GUI.
 * @param schedule to obtain the number of feed of the feeder schedule.
 * @param isSelected specify if the user is currently selecting this text.
 */
void displayNumberOfFeedText(const feedingSchedule* schedule, int isSelected){
    char numberOfFeed[LINE_SIZE];

    // Determines the colour of the text depending on if they are being selected.
    if (isSelected){
        displayColour("black","white");
    }

    else{
        displayColour("white","#5F9EA0");
    }

    // Convert the number of feed integer to string then display on the GUI.
    sprintf(numberOfFeed, "%02d",schedule -> numberOfFeed);
    displayText(CHAR_WIDTH*14,CHAR_HEIGHT*2,numberOfFeed,1);
}


/**
 * Run the schedule time menu operation depending on what the user is currenty selecting.
 * It allows the user to modify day of week, number of feed and time of the feeding schedule.
 * User can save schedule to save text file or exit the schedule time menu.
 * @param scheduleTimeTexts contains
 * @param dayTexts
 * @param feeder
 * @param schedule
 * @param currentOption
 * @return
 */
int selectSetupScheduleTimeOption(textData* scheduleTimeTexts, textData* dayTexts, feederSystem* feeder, feedingSchedule* schedule, int* currentOption){
    void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime);

    switch(*currentOption){
        case 0:
            runningModifyDayOfTheWeekText(scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
        break;

        case 1:
            runningModifyNumberOfFeedText(scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;

        case 2:
            incrementScheduleTimeFunctionPtr = &incrementScheduleHour;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;

        case 3:
            incrementScheduleTimeFunctionPtr = &incrementScheduleMinute;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;
        case 4:
            incrementScheduleTimeFunctionPtr = &incrementScheduleSecond;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;

        case 5:
            return 0;

        case 6:
            addSchedule(feeder, schedule);
            displayOptionText(&scheduleTimeTexts[1],0);
            msleep(500);
            displayOptionText(&scheduleTimeTexts[1],1);
            break;

        default:
            exit(1);
    }

    return 1;
}


void runningModifyDayOfTheWeekText(const textData* scheduleTimeTexts, textData* dayTexts, feedingSchedule* scheduleData, const int* currentOption, feederSystem* feeder){
    char* result;
    int running_modify_day_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
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



void runningModifyNumberOfFeedText(const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* feedingSchedule, const int* currentOption, feederSystem* feeder){
    char* result;
    int running_modify_number_of_feed_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer, feeder) || checkFeederSchedule(feeder)) {
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


void runningModifyScheduleTimeText(void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime), textData* scheduleTimeTexts, textData* dayTexts, feedingSchedule* feedingSchedule, int* currentOption, feederSystem* feeder){
    char* result;
    int running_modify_schedule_time_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
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





