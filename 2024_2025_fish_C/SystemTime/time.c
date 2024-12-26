#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "time.h"
#include "dateTime.h"
#include "userControl.h"
#include "menu.h"
#include "feeder.h"
#include "file.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define LINE_SIZE 80


/**
 * Displays and run the setup time menu.
 * Allows the user to set and save the system time to their desired time.
 * @param feeder is the feeder system, while the user change time it will check to feed the fish.
 */
void runningSetupTimeMenu(feederSystem* feeder) {
    char* result;
    int running_setup_time = 1;
    int prev_second = clockSecond();
    int AFKTimer = 0;
    int currentOption = 0;

    textData* setupTimeTexts = setupTimeMenuTexts();
    systemDateTime* dateTime = getSystemDateTime();

    displaySetupTime(dateTime,setupTimeTexts ,&currentOption);

    printf("Short press to navigate, long press to change value\n");
    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        // Check if the system time has been updated.
        if(isTimeUpdated(&prev_second)){
            // Blank the display if the user has been AFK (away from keyboard) for more than 60 second.
            // and check if it's feeding time for the fishes,
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)){
                displaySetupTime(dateTime,setupTimeTexts, &currentOption);
            }
        }

        // Check if the button has been long pressed.
        if (isLongPressed(result, &AFKTimer)){
            // Run the setup time menu operations depending on what the user is currently selecting.
            running_setup_time = selectSetupTimeOption(&currentOption,setupTimeTexts, dateTime ,feeder);
            free(result);
        }

        // Check if the button has been short pressed.
        if (isShortPressed(result, &AFKTimer)){
            // Allows the user to navigate in between set up time menu texts.
            currentOption = navigateSetupTime(dateTime,setupTimeTexts, currentOption);
        }

    }while(running_setup_time);
    
    free(setupTimeTexts);
    free(dateTime);
}


/**
 * Display setup time menu texts.
 * @param systemDateTime contains the user created system date time.
 * @param setupTimeTexts contains the menu's pre-defined text and layout.
 * @param currentOption specify what the user is currently selecting on the menu.
 */
void displaySetupTime(systemDateTime* systemDateTime, textData* setupTimeTexts, int* currentOption) {
    displayColour("white", "black");
    displayClear();

    // Display the boxes around the date and time values.
    displayDateBox();
    displayTimeBox();

    // Display date and time on the GUI
    displaySystemDateTime(systemDateTime,currentOption);

    // Displays the Exit and Save text on the GUI.
    displayOptionText(&setupTimeTexts[0],0);
    displayOptionText(&setupTimeTexts[1],0);
}


/**
 * Displays the user created system date time for the setup time menu.
 * @param systemDateTime contains user created system date time values such as day, month, year, hour, minute and second.
 * @param currentOption specify what text the user is currently selected on the menu.
 */
void displaySystemDateTime(systemDateTime* systemDateTime, int* currentOption){
    // Displays the date on the menu.
    displayDay(systemDateTime -> date,*currentOption == 0);
    displayMonth(systemDateTime -> date,*currentOption == 1);
    displayYear(systemDateTime -> date, *currentOption == 2);

    // Displays the time on the menu.
    displayHour(systemDateTime -> time,*currentOption == 3);
    displayMinute(systemDateTime -> time,*currentOption == 4);
    displaySecond(systemDateTime -> time,*currentOption == 5);
}


/**
 * Set up the setup time menu texts by pre-defining the menu text for the GUI.
 * @return an array of set up time menu texts.
 */
textData* setupTimeMenuTexts(){
    const int n = 2;
    textData* textDataPtr = malloc(sizeof(textData)*n);
    textDataPtr[0] = setupTextData(0,7,"Exit", 1,"red","black");
    textDataPtr[1] = setupTextData(17,7,"Save", 1,"#7CFC00","black");
    return textDataPtr;
}


/**
 * Run the setup time menu options depending what the user what selecting on at the time.
 * This allows the user to change the date or time of the system.
 * User can also exit out of the menu of save the user created date and time system as the current system.
 * @param currentOption specify what the user is currently selected on the setup time menu.
 * @param setupTimeTexts contains the Exit and save texts of the setup time menu.
 * @param dateTime is the user created date and time.
 * @param feeder is the feeder system to keep checking if the fishes needed to be fed performing operations.
 * @return 1 if the user selected an operations other than exit the program.
 */
int selectSetupTimeOption(int *currentOption, textData* setupTimeTexts, systemDateTime* dateTime, feederSystem* feeder){
    // It's a function pointer which allows the user to increment date and time values.
    void (*incrementFunction)(systemDateTime*);

    switch(*currentOption){
        case 0:
            // Allows the user to modify the day values on the user created date.
            incrementFunction = &incrementDay;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption, feeder);
            break;

        case 1:
            // Allows the user to modify the month values on the user created date.
            incrementFunction = &incrementMonth;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption, feeder);
            break;

        case 2:
            // Allows the user to modify the year values on the user created date.
            incrementFunction = &incrementYear;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption, feeder);
            break;

        case 3:
            // Allows the user to modify the hour values on the user created time.
            incrementFunction = &incrementHour;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption, feeder);
            break;

        case 4:
            // Allows the user to modify the minute values on the user created time.
            incrementFunction = &incrementMinute;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption, feeder);
            break;

        case 5:
            // Allows the user to modify the second values on the user created time.
            incrementFunction = &incrementSecond;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption, feeder);
            break;

        case 6:
            // User exit the setup time menu.
            return 0;

        case 7:
            displayOptionText(&setupTimeTexts[1],0);
            msleep(250);
            displayOptionText(&setupTimeTexts[1],1);

            // System is now the user created date and time.
            loadClock(dateTime);
            // Saves the user created date and time
            saveSystemDateTime(dateTime);
            break;

        default:
            exit(1);
    }

    // User selected time menu operation that is not exit the menu.
    return 1;
}


/**
 * Once the user selected date time values on the setup time menu.
 * The user can now increment the day, month, year, hour, minute and second.
 * @param incrementFunction holds the function to increment the value that they have selected.
 * @param setupTimeTexts contains Save and Exit texts of the time menu.
 * @param dateTime is the user created date time values.
 * @param currentOption specify what the user is currently selected on the menu.
 * @param feeder allow feeding schedules to be kept checking to ensure the fishes are fed.
 */
void runningModifyDateTimeValue(void (*incrementFunction)(systemDateTime*), textData* setupTimeTexts, systemDateTime* dateTime, int *currentOption, feederSystem* feeder){
    int running_changing_date_ime = 1;
    char* result;

    int AFKTimer = 0;
    int prev_second =0;

    displaySelected(1);
    printf("Short press to increment value, long press to confirm value modification\n");
    do{
        msleep(500L);
        result = buttonState();

        // Checks if the system time has been updated.
        if(isTimeUpdated(&prev_second)){
            // Blanks the display after the user has been AFK for 60 seconds
            // Checks if it's feeding time for the fishes.
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
                displaySetupTime(dateTime, setupTimeTexts, currentOption);
                displaySelected(1);
            }
        }

        // Check if the button has been long pressed.
        if (isLongPressed(result, &AFKTimer)){
            displaySelected(0);
            
            // Exit the user from modification of date time value.
            running_changing_date_ime = 0;
            free(result);
        }

        
        if (isShortPressed(result, &AFKTimer)){
            // Increment the date or time specific value
            // Such as day, month, year, hour, minute and second.
            incrementFunction(dateTime);
        }

    }while(running_changing_date_ime);
}


/**
 * Allows the user to navigate in between the setup time menu options.
 * @param systemDateTime is the user created date and time values.
 * @param setupTimeTexts contains setup time menu texts such as Exit and Save.
 * @param currentOption specify what the user is currently selected on.
 * @return 
 */
int navigateSetupTime(systemDateTime* systemDateTime, textData* setupTimeTexts, int currentOption){
    switch(currentOption){
        case 0:
            // User selected month value on the menu.
            displayDay(systemDateTime -> date,0);
            displayMonth(systemDateTime -> date,1);
            break;

        case 1:
            // User selected year value on the menu.
            displayMonth(systemDateTime -> date,0);
            displayYear(systemDateTime -> date,1);
            break;

        case 2:
            // User selected hour value on the menu.
            displayYear(systemDateTime -> date,0);
            displayHour(systemDateTime -> time,1);
            break;

        case 3:
            // User selected minute value on the menu.
            displayHour(systemDateTime -> time,0);
            displayMinute(systemDateTime -> time,1);
            break;

        case 4:
            // User selected second value on the menu.
            displayMinute(systemDateTime -> time,0);
            displaySecond(systemDateTime -> time,1);
            break;

        case 5:
            // User selected exit option on the menu.
            displaySecond(systemDateTime -> time,0);
            displayOptionText(&setupTimeTexts[0],1);
            break;

        case 6:
            // User selected save option on the menu.
            displayOptionText(&setupTimeTexts[0],0);
            displayOptionText(&setupTimeTexts[1],1);
            break;

        case 7:
            // User selected day value on the menu.
            displayOptionText(&setupTimeTexts[1],0);
            displayDay(systemDateTime -> date,1);
            break;

        default:
            // An error occurred where the program doesn't know what the user
            // is currently selected so exit the program.
            exit(1);
    }

    // Setup time menu increment the user selected option.
    return (currentOption+1)%8;
}