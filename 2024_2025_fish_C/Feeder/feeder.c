#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "fish.h"
#include "feeder.h"
#include "dateTimeStruct.h"
#include "userControl.h"
#include "feederSchedule.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80


/**
 * Display and runs the feeder menu section of the program.
 * @param feeder contains all the data of the fish feeder system.
 */
void runningFeederMenu(feederSystem* feeder){
    int running_feeder_menu = 1; // continue flag
    int AFKTimer = 0;
    int prev_second = clockSecond();
    int currentOption = 0;

    // Set up the feeder menu texts
    textData* feederMenuTexts = setupFeederMenuTexts();

    // Display the menu texts.
    displayFeederMenu(feederMenuTexts,&currentOption);
    printf("Short press the button to navigate, long to confirm\n");

    do{
        // check for the button state every half second
        msleep(500L);
        // get the button state from the JavaFX application
        char* result = buttonState();

        // Checks if a second has past.
        if(isTimeUpdated(&prev_second)){
            // Blanks the display after 1 minute of the button no being pressed
            // Checks if it's feeding time.
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
                displayFeederMenu(feederMenuTexts,&currentOption);
            }

            // display the new clock time.
            displayClockTime();
        }

        // Checks if a button long pressed happen.
         if (isLongPressed(result, &AFKTimer)) {
             // Run one of the feeder menu options. Including leaving the feeder menu.
             running_feeder_menu = runFeederMenuOption(feeder, currentOption);
             free(result);
        }

        // Checks if a button short pressed happens.
        if (isShortPressed(result, &AFKTimer)) {
            navigateFeederMenu(feederMenuTexts, &currentOption);
        }

    }while(running_feeder_menu);

    free(feederMenuTexts);
}


/**
 * Creates an array containing text options data of the feeder menu.
 * @return an array with all the feeder menu text options.
 */
textData* setupFeederMenuTexts(){
    int n = 5;
    textData* feederMenuTexts = malloc(sizeof(textData) * n);
    feederMenuTexts[0] = setupTextData(0,0,"Feed Now",1,"white","black");
    feederMenuTexts[1] = setupTextData(0,1,"Auto Feed",1,"white","black");
    feederMenuTexts[2] = setupTextData(0,2,"Skip Next Feed",1,"white","black");
    feederMenuTexts[3] = setupTextData(0,3,"Pause",1,"white","black");
    feederMenuTexts[4] = setupTextData(0,5,"Exit",1,"red","black");

    return feederMenuTexts;
}


/**
 * Creates a default feeder system with default values.
 * Designed if the program doesn't have any schedules saved.
 * @return a feeder system with default values.
 */
feederSystem* setupFeederSystem(){
    feederSystem* feederSystem = malloc(sizeof(feederSystem));

    // Default feeder system comes with no schedules.
    feederSystem -> arrayOfSchedules = NULL;
    feederSystem -> amountOfSchedules = 0;

    // Indicates which schedules is next on the feeder system.
    // Set to -1 because there is no schedules.
    feederSystem -> nextFeedTimeIndex = -1;
    feederSystem -> totalNumberOfFeed = 0;

    // Default feeder system operating mode start with automatic feeding.
    feederSystem -> operatingMode = 'A';
    
    return feederSystem;
}


/**
 * Displays all the feeder menu texts options and also takes into account where the user is currently selecting
 * and displays that specific text box at a different colour so the user knows where they are selecting.
 * useful when the user blanks mid selecting and can continue straight away.
 * @param feederMenuTexts contains data of all feeder menu texts.
 * @param currentOption displays the selected feeder menu text different from other options.
 */
void displayFeederMenu(const textData* feederMenuTexts, const int* currentOption){
    // Clear the screens.
    displayColour("white","black");
    displayClear();

    // Display all feeder menu texts options.
    for (int i = 0; i < 5; i++){
        displayOptionText(&feederMenuTexts[i],*currentOption == i);
    }

    // Displays the clock time within the feeder menu.
    displayClockTime(clockSecond());
}


/**
 * Runs the feeder menu options depending on where the user is selecting.
 * @param feeder contains data of the feeder system.
 * @param currentOption indicates where the user is selecting.
 * @return 
 */
int runFeederMenuOption(feederSystem* feeder, int currentOption){
    switch(currentOption){
        case 0:
            // Will feed the fishes in the fish tank.
            feedNow();
            break;

        case 1:
            // Change the feeder system operating mode to AUTO.
            feederOperatingModeAuto(feeder);
            break;

        case 2:
            // Change the feeder system operating mode to SKIP the next feeding schedules
            // then changing the operating mode back to auto.
            feederOperatingModeSkipNextFeed(feeder);
            break;

        case 3:
            // Change the feeder system operating mode to PAUSE.
            feederOperatingModePause(feeder);
            break;

        case 4:
            // Exits the feeder menu.
            return 0;

        default:
            // Exits the program if any error occurred.
            exit(1);
    }

    // Continues running the feeder menu.
    return 1;
}


/**
 * Main component of the feeder system.
 * To feed the fishes.
 */
void feedNow(){
    // step the motor through a complete rotation (360 degrees)
    for (int i = 0; i < 360; i++) {
        motorStep(); // step the motor in the JavaFX application
        msleep(30L); // can't go too fast.
    }
}


/**
 * Change the feeder system operating mode to AUTO
 * @param feeder contains the feeder system operating mode.
 */
void feederOperatingModeAuto(feederSystem* feeder){
    feeder -> operatingMode = AUTO;
}


/**
 * Change the feeder system operating mode to SKIP
 * @param feeder contains the feeder system operating mode.
 */
void feederOperatingModeSkipNextFeed(feederSystem* feeder){
    feeder -> operatingMode = SKIP;
}


/**
 * Change the feeder system operating mode to PAUSED
 * @param feeder contains the feeder system operating mode.
 */
void feederOperatingModePause(feederSystem* feeder){
    feeder -> operatingMode = PAUSE;
}


/**
 * Allows the user to navigate in between the feeder menu options
 * @param feederMenuTexts contains the all the feeder menu text options data.
 * @param currentOption indicate where the user is currently selected on the feeder menu.
 */
void navigateFeederMenu(const textData* feederMenuTexts, int* currentOption){
    // Stores the next menu options indicator.
    int newCurrentOption = ((*currentOption) + 1) % 5;

    // Change the user currently selected option to non-selected colour.
    displayOptionText(&feederMenuTexts[*currentOption],0);

    // Change the next menu option to selected colour.
    displayOptionText(&feederMenuTexts[newCurrentOption],1);

    // Allows the user to continue navigating from the newly selected option.
    *currentOption = newCurrentOption;
}


/**
 * Designed for the start menu to display the next feed time.
 * @param startMenuTexts is the start menu texts data including the next feed time text.
 * @param feeder is the feeder system and used to extract the next feed time.
 */
void displayNextFeedTime(textData* startMenuTexts, feederSystem* feeder){
    // Base next feed text.
    char nextFeedTimeText[] = "Next Feed:   ";
    char nextFeedTimeValue[LINE_SIZE];

    // Grab the index within the feeder system array of schedules,
    // that points to the next feed time schedule.
    int index = feeder -> nextFeedTimeIndex;

    // Checks if there is any schedules at all.
    if (index != -1){
        // Grabs the time values of the schedules that is next for the feed system.
        int hour = feeder -> arrayOfSchedules[index].time.hour;
        int minute = feeder -> arrayOfSchedules[index].time.minute;
        int second = feeder -> arrayOfSchedules[index].time.second;

        // Convert the time values to the time format and string.
        sprintf(nextFeedTimeValue, "%02d:%02d:%02d", hour, minute, second);

        // Append the next feed time values to the base next feed text.
        strcat(nextFeedTimeText, nextFeedTimeValue);

        // Change the start menu next feed time text to an updated one.
        startMenuTexts[1].text = malloc(sizeof(nextFeedTimeText));
        strcpy(startMenuTexts[1].text, nextFeedTimeText);
    }

    else{
        // If there is no next feed time text.
        char noNextTimeText[] = "00:00:00";

        // Change the start menu next feed time text to an updated one.
        startMenuTexts[1].text = malloc(sizeof(noNextTimeText));
        strcat(nextFeedTimeText, noNextTimeText);
        strcpy(startMenuTexts[1].text, nextFeedTimeText);
    }

}


/**
 * Displays the total amount of feeding has been done to fishes.
*  @param startMenuTexts is the start menu texts data including the total number of feed text.
 * @param feeder is the feeder system and used to extract the total number of feed.
 */
void displayTotalNumberOfFeeds(textData* startMenuTexts, feederSystem* feeder){
    char numberOfFeedsText[] = "Number Of Feed:   ";
    char text[LINE_SIZE];

    // Grab the total number of feed from the feeder system in string format.
    sprintf(text, "%d", feeder -> totalNumberOfFeed);

    // Concat base number of feed text and the value.
    strcat(numberOfFeedsText, text);

    // Updates the start menu total number of feed text.
    startMenuTexts[2].text = malloc(sizeof(numberOfFeedsText));
    strcpy(startMenuTexts[2].text, numberOfFeedsText);
}


/**
 * Displays the operating mode of the feeder system.
 * @param startMenuTexts is the start menu texts data including the operating mode text.
 * @param feeder is the feeder system and used to extract operating mode.
 */
void displayOperatingMode(textData* startMenuTexts, feederSystem* feeder){
    char operatingModeText[] = "Operating Mode:   ";

    // Grab the char operating mode and concat to the base operating mode text.
    sprintf(operatingModeText, "%s%c", operatingModeText, feeder -> operatingMode);

    // Updates the start menu operating mode text.
    startMenuTexts[3].text = malloc(strlen(operatingModeText)+1);
    strcpy(startMenuTexts[3].text, operatingModeText);
}


/**
 * Checks the feeder schedules and see if it's feeding time.
 * A schedule day of week and time must match with the system day of week and time in order to feed the fish.
 * Feeder system must follow the user desired operations depending on the operating mode
 * After a schedule has happened or passed it will update the next feed time schedule to keep track off.
 * @param feeder is the feeder system which contains an array of schedules and
 * @return 1 if the next feed time schedule has been updated. Otherwise, 0.
 */
int checkFeederSchedule(feederSystem* feeder){
    // Grab the current operating mode and amount of schedules from the feeder system
    operatingEnum operatingMode = feeder->operatingMode;
    int amountOfSchedules = feeder -> amountOfSchedules;

    // Exit the check feeder schedule if there is no schedules.
    // To avoid redundant check operating when there is nothing to check.
    if (amountOfSchedules == 0){
        return 0;
    }

    // Grab the next feeding schedules from the feeder system.
    feedingSchedule* nextFeedingSchedule = &feeder->arrayOfSchedules[feeder->nextFeedTimeIndex];

    // If the next schedule time does not match the with the current time then exit function.
    if (!isFeedingTime(nextFeedingSchedule)){
        return 0;
    }

    // Checks the operating mode and depending on the mode it will either feed the fishes,
    // skip the current schedule or just not feed.
    checkOperatingMode(nextFeedingSchedule, feeder, operatingMode);

    // Update the next feeding schedule.
    updateNextFeedTimeIndex(feeder);
    return 1;

}


/**
 * Checks the operating mode and depending on the operating mode it will either feed the fish,
 * skip to next feeding schedule or just update next feed time schedule.
 * @param nextFeedingSchedule is next schedule in line for the feeding system.
 * @param feeder is the feeder system to extract the number of total feed to increment by one if fishes are fed.
 * @param operatingMode different operations are performed based on the operating mode.
 */
void checkOperatingMode(feedingSchedule* nextFeedingSchedule, feederSystem* feeder, operatingEnum operatingMode){
    // If the feeder system is on AUTO mode then the fishes will be fed.
    if (operatingMode == AUTO){
        printf("FEEDING TIME!!!\n");

        int numberOfFeedNow = nextFeedingSchedule -> numberOfFeed;

        // Feed the fishes based on the schedule specified amount of feeding.
        for (int i=0; i<numberOfFeedNow; i++){
            // Spins the rotor to feed the fishes.
            feedNow();

            // Increment the total number of feeding in the feed system data.
            feeder -> totalNumberOfFeed += 1;
        }
    }

    // If the feeder system is on SKIP
    else if(operatingMode == SKIP){
        // Change the operating mode back to AUTO.
        feeder -> operatingMode = AUTO;
    }
}


/**
 * Checks if the feeder schedule day of week and time is same as the system day of week and time.
 * @param feederSchedule is the current next feeder schedule.
 * @return 1 if the feeder schedule data matches with system day of week and time.
 */
int isFeedingTime(feedingSchedule* feederSchedule){
    systemTime* feederTime = &feederSchedule->time;
    // clockDayOfWeek() has been +6 then %7 was because I created
    // dayOfWeek from 0 = Monday to 6 = Sunday.
    // To allow for better insertion sort performance updateNextFeedTimeIndex() in feederSchedule.c
    return (feederSchedule->dayOfWeek == ((clockDayOfWeek() + 6) %7) && feederTime->hour == clockHour() && feederTime -> minute == clockMinute() && feederTime -> second == clockSecond());
}






