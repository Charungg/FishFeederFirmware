#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "setUpTime.h"
#include "time.h"
#include "userControl.h"
#include "menu.h"
#include "file.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define LINE_SIZE 80


void runningSetupTime() {
    char* result;
    int running_setup_time = 1;
    int prev_second = clockSecond();
    int AFKTimer = 0;
    int currentOption = 0;

    printf("Short press to navigate, long press to change value\n");
    systemDateTime time = getSystemDateTime();

    displaySetupTime(time);

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupTime(time);
                currentOption = 0;
            }
        }


        if (isLongPressed(result)){
            running_setup_time = selectDateTimeOption(&currentOption, &time);
            AFKTimer = 0;
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        if (isShortPressed(result)){
            currentOption = navigateSetupTime(currentOption, time);
            AFKTimer = 0;
        }

    }while(running_setup_time);
}


void displaySetupTime(systemDateTime time) {
    displayColour("white", "black");
    displayClear();

    displaySystemDateTime(time);

    displaySave(0);
    displayExit(0,7);
}


void displaySystemDateTime(systemDateTime time){
    char text[LINE_SIZE];
    char* dateTimeFormat = "%02d";
    char* yearFormat = "%04d";

    displayDateBox(text);
    displayTimeBox(text);

    displayDay(dateTimeFormat, time, 1);
    displayMonth(dateTimeFormat, time, 0);
    displayYear(yearFormat, time, 0);

    displayHour(dateTimeFormat, time, 0);
    displayMinute(dateTimeFormat, time, 0);
    displaySecond(dateTimeFormat, time, 0);
}


void displayDay(const char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.day, isSelected,8,2);
}


void displayMonth(const char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.month, isSelected,11,2);
}


void displayYear(const char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.year, isSelected,14,2);
}


int selectDateTimeOption(int *currentOption, systemDateTime* time){
    void (*incrementFunction)(systemDateTime*);

    switch(*currentOption){
    case 0:
        displaySelected();
        incrementFunction = &incrementDay;
        runningModifyDateTimeValue(incrementFunction, time, currentOption);
        break;

    case 1:
        displaySelected();
        incrementFunction = &incrementMonth;
        runningModifyDateTimeValue(incrementFunction, time, currentOption);
        break;

    case 2:
        displaySelected();
        incrementFunction = &incrementYear;
        runningModifyDateTimeValue(incrementFunction, time, currentOption);
        break;

    case 3:
        displaySelected();
        incrementFunction = &incrementHour;
        runningModifyDateTimeValue(incrementFunction, time, currentOption);
        break;

    case 4:
        displaySelected();
        incrementFunction = &incrementMinute;
        runningModifyDateTimeValue(incrementFunction, time, currentOption);
        break;

    case 5:
        displaySelected();
        incrementFunction = &incrementSecond;
        runningModifyDateTimeValue(incrementFunction, time, currentOption);
        break;

    case 6:
        return 0;

    case 7:
        displayColour("#7FFF00", "white");
        displayText(CHAR_WIDTH*17,CHAR_HEIGHT*7,"Save",1);
        msleep(250);
        displaySave(1);
        clockSet(time -> second, time -> minute, time -> hour, time -> day, time -> month, time -> year);
        saveSystemDateTime(*time);
        break;
    }

    return 1;
}


void runningModifyDateTimeValue(void (*incrementFunction)(systemDateTime*),systemDateTime* time, int *currentOption){
    int running_changing_date_ime = 1;
    char* result;

    int AFKTimer = 0;
    int prev_second =0;


    do{
        msleep(500L);
        result = buttonState();

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupTime(*time);
                *currentOption = 0;
                running_changing_date_ime = 0;
            }
        }

        if (isLongPressed(result)){
            clearDisplaySelected();
            running_changing_date_ime = 0;
        }

        if (isShortPressed(result)){
            incrementFunction(time);
        }

    }while(running_changing_date_ime);
}


void incrementDay(systemDateTime* time){
    char* dateTimeFormat = "%02d";
    int maximumDayOfMonths[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if ((time -> year) % 4 == 0){
        maximumDayOfMonths[1] = 29;
    }

    time -> day++;

    if ((time -> day) > maximumDayOfMonths[(time -> month) -1]){
        time -> day = 1;
    }
    displayDay(dateTimeFormat, *time, 1);
}

void incrementMonth(systemDateTime* time){
    char* dateTimeFormat = "%02d";

    time -> month++;
    if ((time -> month) == 13){
        time -> month = 1;
    }

    displayMonth(dateTimeFormat, *time, 1);
}


void incrementYear(systemDateTime* time){
    char* yearFormat = "%04d";

    time -> year = (((time -> year) + 1) % 9999);
    displayYear(yearFormat, *time, 1);
}


void clearDisplaySelected(){
    displayColour("white", "black");
    displayClearArea(0,0,CHAR_WIDTH*8,CHAR_HEIGHT*1);
}


int navigateSetupTime(int currentOption, systemDateTime time){
    char* dateTimeFormat = "%02d";
    char* yearFormat = "%04d";

    switch(currentOption){
        case 0:
            displayDay(dateTimeFormat,time,0);
            displayMonth(dateTimeFormat,time,1);
            break;
        case 1:
            displayMonth(dateTimeFormat,time,0);
            displayYear(yearFormat,time,1);
            break;
        case 2:
            displayYear(yearFormat,time,0);
            displayHour(dateTimeFormat,time,1);
            break;
        case 3:
            displayHour(dateTimeFormat,time,0);
            displayMinute(dateTimeFormat,time,1);
            break;
        case 4:
            displayMinute(dateTimeFormat,time,0);
            displaySecond(dateTimeFormat,time,1);
            break;
        case 5:
            displaySecond(dateTimeFormat,time,0);
            displayExit(1,7);
            break;
        case 6:
            displayExit(0,7);
            displaySave(1);
            break;
        case 7:
            displaySave(0);
            displayDay(dateTimeFormat,time,1);
            break;
        default:
            exit(1);
    }


    return (currentOption+1)%8;
}