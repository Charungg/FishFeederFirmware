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
    feedingSchedule* feedingSchedule = malloc(sizeof(*feedingSchedule));
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
 * Run the schedule time menu operation depending on what the user is selecting.
 * It allows the user to modify day of week, number of feed and time of the feeding schedule.
 * User can save schedule to save text file or exit the schedule time menu.
 * @param scheduleTimeTexts contains Save and Exit text for the GUI
 * @param dayTexts contains Monday - Sunday texts for the GUI
 * @param feeder contains an array of schedules where user created feeding schedule is placed
 * @param schedule contains the day of week, number of feed and time of the feeding schedule.
 * @param currentOption specify which text the user is currently selecting on.
 * @return
 */
int selectSetupScheduleTimeOption(const textData* scheduleTimeTexts, const textData* dayTexts, feederSystem* feeder, feedingSchedule* schedule, const int* currentOption){
    // Void function pointer which is designed to update the time of the feeding schedule values such as hour, minute and second.
    void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime);

    switch(*currentOption){
        case 0:
            // Allows the user to change the day of the week of the feeding schedule.
            runningModifyDayOfTheWeekText(scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
        break;

        case 1:
            // Allows the user to change the number of feed of the feeding schedule.
            runningModifyNumberOfFeedText(scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;

        case 2:
            // Allows the user to increment the hour of the feeding schedule.
            incrementScheduleTimeFunctionPtr = &incrementScheduleHour;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;

        case 3:
            // Allows the user to increment the minute of the feeding schedule.
            incrementScheduleTimeFunctionPtr = &incrementScheduleMinute;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;
        case 4:
            // Allows the user to increment the second of the feeding schedule.
            incrementScheduleTimeFunctionPtr = &incrementScheduleSecond;
            runningModifyScheduleTimeText(incrementScheduleTimeFunctionPtr, scheduleTimeTexts, dayTexts, schedule, currentOption, feeder);
            break;

        case 5:
            // Exit the schedule time menu.
            return 0;

        case 6:
            // User created schedule is saved on the feeder system.
            addSchedule(feeder, schedule);

            // Display the save Text blinking slightly to indicate activation.
            displayOptionText(&scheduleTimeTexts[1],0);
            msleep(500);
            displayOptionText(&scheduleTimeTexts[1],1);
            break;

        default:
            // If an error has occurred where the program cannot recognise where the user is selecting.
            // Close the program
            exit(1);
    }

    // Continue the schedule time menu.
    return 1;
}


/**
 * Allows the user to modify the day of the week on the feeding schedule.
 * @param scheduleTimeTexts contains the Save and Exit text for the GUI.
 * @param dayTexts contains Monday - Sunday text for the GUI.
 * @param schedule contains day of the week data of the feeding schedule.
 * @param currentOption specify where the user is currently selecting.
 * @param feeder is the feeder system which contains array of schedules.
 */
void runningModifyDayOfTheWeekText(const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* schedule, const int* currentOption, feederSystem* feeder){
    char* result;
    int running_modify_day_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        // Check if the system time has been updated.
        if(isTimeUpdated(&prev_second)){
            // Check if the user has been AFK for more than 60 second, if so blank the screen
            // and check if it's feeding time.
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
                // If the display blanked or feeding time has happened then redisplay schedule time menu.
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,schedule,currentOption);
                displaySelected(1);
            }
        }

        // Check if the button has been long pressed.
        if (isLongPressed(result, &AFKTimer)){
            // Exit the program with a modified day of the week data on the feeding schedule.
            running_modify_day_menu = 0;
            displaySelected(0);
            free(result);
        }

        // Check if the button has been short pressed.
        if (isShortPressed(result, &AFKTimer)){
            // Increment the day of the week text on the GUI and feeding schedule data.
            incrementDayOfTheWeek(schedule,dayTexts);
        }
        
    }while(running_modify_day_menu);
}


/**
 * Increment the feeding schedule day of the week value and display newly updated day of the week text.
 * @param schedule is the feeding schedule which contains the day of the week value.
 * @param dayTexts contains Monday - Sunday day of the week texts for the GUI.
 */
void incrementDayOfTheWeek(feedingSchedule* schedule, const textData* dayTexts){
    (schedule -> dayOfWeek) = ((schedule->dayOfWeek + 1) % 7);
    displayOptionText(&dayTexts[schedule -> dayOfWeek], 1);
}


/**
 * Allows the user to modify the number of feed of the feeding schedule can do.
 * @param scheduleTimeTexts contains the Save and Exit text for the GUI.
 * @param dayTexts contains Monday - Sunday text for the GUI.
 * @param schedule 
 * @param currentOption 
 * @param feeder 
 */
void runningModifyNumberOfFeedText(const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* schedule, const int* currentOption, feederSystem* feeder){
    char* result;
    int running_modify_number_of_feed_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        // Check if the system time has been updated.
        if(isTimeUpdated(&prev_second)){
            // Blank the display if the user has been AFK for more than 60 second and check if its feeding time.
            if(isUserAFK(&AFKTimer, feeder) || checkFeederSchedule(feeder)) {
                // If the display blanked or feeding time has happened then redisplay schedule time menu.
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,schedule,currentOption);
                displaySelected(1);
            }
        }

        // Check if button has been long pressed.
        if (isLongPressed(result, &AFKTimer)){
            // Exit the program with a modified number of feed data on the feeding schedule.
            running_modify_number_of_feed_menu = 0;
            displaySelected(0);
            free(result);
        }

        // Check if button has been short pressed.
        if (isShortPressed(result, &AFKTimer)){
            // Increment the number of feed text on the GUI and feeding schedule data.
            incrementNumberOfFeed(schedule);
        }
        
    }while(running_modify_number_of_feed_menu);
}


/**
 * Increment the feeding schedule number of feed and display the updated number of feed on the schedule time menu.
 * @param schedule is the user created feeding schedule.
 */
void incrementNumberOfFeed(feedingSchedule* schedule){
    int *numberOfFeedPtr = &schedule -> numberOfFeed;

    // Increment the number of feed data on the feeding schedule.
    *numberOfFeedPtr = (*numberOfFeedPtr + 1) % 100;

    // Display the updated number of feed on the schedule time menu.
    displayNumberOfFeedText(schedule,1);
}


/**
 * Allows the user to increment feeding schedule time values such as hour, minute and second.
 * @param incrementScheduleTimeFunctionPtr is a function pointer of feeding schedule time value increment function.
 * @param scheduleTimeTexts contains Save and Exit texts for the schedule time menu.
 * @param dayTexts contains Monday - Sunday day texts for the schedule time menu.
 * @param schedule contains feeding schedule time values such as hour, minute and second.
 * @param currentOption specify what the user is currently selecting on the schedule time menu.
 * @param feeder is a feeder system.
 */
void runningModifyScheduleTimeText(void(*incrementScheduleTimeFunctionPtr)(systemTime* systemTime), const textData* scheduleTimeTexts, const textData* dayTexts, feedingSchedule* schedule, const int* currentOption, feederSystem* feeder){
    char* result;
    int running_modify_schedule_time_menu = 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        // Check if the system time has been updated.
        if(isTimeUpdated(&prev_second)){
            // Blank the display if the user has been AFK for 60 second or more.
            // Check if it's feeding time for the fishes.
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
                // If the display blanked or feeding time has happened then redisplay schedule time menu.
                displaySetupScheduleTimeTexts(scheduleTimeTexts,dayTexts,schedule,currentOption);
                displaySelected(1);
            }
        }

        // Check if the button has been long pressed.
        if (isLongPressed(result, &AFKTimer)){
            // Exit the program with a modified time data on the feeding schedule.
            running_modify_schedule_time_menu = 0;
            displaySelected(0);
            free(result);
        }

        // Check if the button has been short pressed.
        if (isShortPressed(result, &AFKTimer)){
            // Increment the feeding schedule time values such as hours, minute and second.
            incrementScheduleTimeFunctionPtr(&schedule->time);
        }

    }while(running_modify_schedule_time_menu);
}


/**
 * Increment the feeding schedule hour by 1 and display the updated hour.
 * @param systemTime contains the feeding schedule hour value.
 */
void incrementScheduleHour(systemTime* systemTime){
    int* scheduleSecondHour = &systemTime -> hour;
    *scheduleSecondHour = (*scheduleSecondHour + 1) % 24;

    displayHour(systemTime,1);
}


/**
 * Increment the feeding schedule minute by 1 and display the updated minute.
 * @param systemTime contains the feeding schedule minute value.
 */
void incrementScheduleMinute(systemTime* systemTime){
    int* scheduleSecondMinute = &systemTime -> minute;
    *scheduleSecondMinute = (*scheduleSecondMinute + 1) % 60;

    displayMinute(systemTime,1);
}


/**
 * Increment the feeding schedule second by 1 and display the updated second.
 * @param systemTime contains the feeding schedule second value.
 */
void incrementScheduleSecond(systemTime* systemTime){
    int* scheduleSecond = &systemTime -> second;
    *scheduleSecond =  (*scheduleSecond + 1) % 60;

    displaySecond(systemTime,1);
}


/**
 * Allows the user to navigate in between text within the schedule time menu.
 * @param scheduleTimeTexts contains Save and Exit text.
 * @param dayTexts contains Monday - Sunday day of the week texts.
 * @param schedule is the user created feeding schedule which contain time values.
 * @param currentOption specifies what the user is currently selecting in the schedule time menu texts.
 */
void navigateSetupScheduleTime(const textData* scheduleTimeTexts, const textData* dayTexts, const feedingSchedule* schedule, int* currentOption){
    switch(*currentOption){
        case 0:
            // User selected number of feed text on the menu.
            displayOptionText(&dayTexts[schedule->dayOfWeek], 0);
            displayNumberOfFeedText(schedule, 1);
            break;

        case 1:
            // User selected feeding schedule hour value on the menu.
            displayNumberOfFeedText(schedule, 0);
            displayHour(&schedule -> time, 1);
            break;

        case 2:
            // User selected feeding schedule minute value on the menu.
            displayHour(&schedule -> time, 0);
            displayMinute(&schedule -> time,1);
            break;

        case 3:
            // User selected feeding schedule second value on the menu.
            displayMinute(&schedule -> time,0);
            displaySecond(&schedule-> time,1);
            break;

        case 4:
            // User selected Exit option on the menu.
            displaySecond(&schedule -> time,0);
            displayOptionText(&scheduleTimeTexts[0],1);
            break;

        case 5:
            // User selected Save option on the menu.
            displayOptionText(&scheduleTimeTexts[0],0);
            displayOptionText(&scheduleTimeTexts[1],1);
            break;

        case 6:
            // User selected day of the week option on the menu.
            displayOptionText(&scheduleTimeTexts[1],0);
            displayOptionText(&dayTexts[schedule->dayOfWeek], 1);
            break;

        default:
            // An error occurred what the user is currently selecting on the menu.
            exit(1);
    }

    // Schedule time menu increment the user selected option.
    *currentOption = ((*currentOption + 1)% 7);
}





