//
// Created by charl on 29/11/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "fish.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 64/8 = 8 HEIGHT
// 128/6 = 21.333^* WIDTH

#define LINE_SIZE 80

void test();

void displaySetupTime(systemTime time) {
    displayColour("white", "black");
    displayClear();

    displaySystemDateAndTime(time);

    displaySave(0);
    displayExit(0,7);
}

void displaySystemDateAndTime(systemTime time){
    char text[LINE_SIZE];
    char* dateTimeFormat = "%02d";
    char* yearFormat = "%04d";

    displaySystemDateAndTimeFormat(text);
    displayDay(dateTimeFormat, time, 1);
    displayMonth(dateTimeFormat, time, 0);
    displayYear(yearFormat, time, 0);

    displayHour(dateTimeFormat, time, 0);
    displayMinute(dateTimeFormat, time, 0);
    displaySecond(dateTimeFormat, time, 0);
}

void displaySystemDateAndTimeFormat(char* text){
    displayColour("white", "grey");

    text = "Date |  /  /";
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*2,text,1);

    text = "Time |  :  :  ";
    displayColour("white", "grey");
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*4,text,1);
}

void displayDay(char* format, systemTime time, int isSelected){
    displayTimeDate(format, time.day, isSelected,8,2);
}

void displayMonth(char* format, systemTime time, int isSelected){
    displayTimeDate(format, time.month, isSelected,11,2);
}

void displayYear(char* format, systemTime time, int isSelected){
    displayTimeDate(format, time.year, isSelected,14,2);
}

void displayHour(char* format, systemTime time, int isSelected){
    displayTimeDate(format, time.hour, isSelected,8,4);
}

void displayMinute(char* format, systemTime time, int isSelected){
    displayTimeDate(format, time.minute, isSelected,11,4);
}

void displaySecond(char* format, systemTime time, int isSelected){
    displayTimeDate(format, time.second, isSelected,14,4);
}

void displayTimeDate(char* format, int time, int isSelected, int width, int height){
    char text[LINE_SIZE];
    isTimeSelected(isSelected);
    snprintf(text, LINE_SIZE, format, time);
    displayText(CHAR_WIDTH*width,CHAR_HEIGHT*height,text,1);
}


systemTime getSystemTime() {
    systemTime time;
    time.day = clockDay();
    time.month = clockMonth();
    time.year = clockYear();
    time.hour = clockHour();
    time.minute = clockMinute();
    time.second = clockSecond();
    return time;
}


void runningSetupTime(char* result) {
    int running_setup_time = 1;
    int currentOption = 0;

    // char text[LINE_SIZE];

    printf("Short press to navigate, long press to change value\n");
    systemTime time = getSystemTime();

    displaySetupTime(time);

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if (isLongPressed(result)){
            running_setup_time = selectDateTimeOption(currentOption, &time);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result)){
            currentOption = navigateSetupTime(currentOption, time);
        }

    }while(running_setup_time);
}

int isLongPressed(char* result){
    return (strcmp(result, "LONG_PRESS") == 0);
}

int isShortPressed(char* result){
    return (strcmp(result, "SHORT_PRESS") == 0);
}


void displaySave(int isSelected) {
    isOptionSelected(isSelected);
    displayText(CHAR_WIDTH*17,CHAR_HEIGHT*7,"Save",1);
}


void isOptionSelected(int select){
    if (select){
        displayColour("black", "white");
    }

    else{
        displayColour("white", "black");
    }
}

void isTimeSelected(int select){
    if (select){
        displayColour("grey", "white");
    }

    else{
        displayColour("white", "grey");
    }
}

int navigateSetupTime(int currentOption, systemTime time){
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
            displayYear(dateTimeFormat,time,0);
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
    }

    return (currentOption+1)%8;
}


int selectDateTimeOption(int currentOption, systemTime* time){
    void (*incrementFunction)(systemTime*, int);

    switch(currentOption){
        case 0:
            displaySelected();
            incrementFunction = incrementDay;
            selectDateTime(incrementFunction, time);
            break;

        case 1:
            displaySelected();
            incrementFunction = incrementMonth;
            selectDateTime(incrementFunction, time);
            break;

        case 2:
            displaySelected();
            incrementFunction = incrementYear;
            selectDateTime(incrementFunction, time);
            break;

        case 3:
            displaySelected();
            incrementFunction = incrementHour;
            selectDateTime(incrementFunction, time);
            break;

        case 4:
            displaySelected();
            incrementFunction = incrementMinute;
            selectDateTime(incrementFunction, time);
            break;

        case 5:
            displaySelected();
            incrementFunction = incrementSecond;
            selectDateTime(incrementFunction, time);
            break;

        case 6:
            return 0;

        case 7:
            displayColour("#7FFF00", "white");
            displayText(CHAR_WIDTH*17,CHAR_HEIGHT*7,"Save",1);
            msleep(250);
            displaySave(1);
            clockSet(time -> second, time -> minute, time -> hour, time -> day, time -> month, time -> year);
            break;
    }

    return 1;
}

void displaySelected(){
    displayColour("white", "black");
    displayText(0,0,"Selected",1);
}

void selectDateTime(void (*incrementFunction)(systemTime*, int), systemTime* time){
    char text[LINE_SIZE];
    char* result;
    int running_select_date_time = 1;

    printf("Short press to increment, long press to confirm\n");
    do{
        msleep(500L);
        result = buttonState();

        if (isLongPressed(result)){
            displayColour("white", "black");
            displayClearArea(0,0,48,8);
            running_select_date_time = 0;
            printf("Saved\n");
            free(result);
        }

        else if (isShortPressed(result)){
            incrementFunction(time,1);

        }

    }while(running_select_date_time);
}

void incrementDay(systemTime* time, int isSelected){
    char* dateTimeFormat = "%02d";
    int maximumDayPerMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if ((time-> year % 4) == 0){
        maximumDayPerMonth[1] = 29;
    }

    time -> day = (time-> day + 1);

    if (time -> day > maximumDayPerMonth[time -> month - 1]){
        time -> day = 1;
    }

    displayDay(dateTimeFormat,*time,isSelected);
}

void incrementMonth(systemTime* time, int isSelected){
    char* dateTimeFormat = "%02d";

    time -> month = ((time -> month + 1) % 12);
    displayMonth(dateTimeFormat,*time,isSelected);
}

void incrementYear(systemTime* time, int isSelected){
    char* yearFormat = "%04d";

    time -> year = ((time -> year + 1) % 9999);
    displayYear(yearFormat,*time,isSelected);
}

void incrementHour( systemTime* time, int isSelected){
    char* dateTimeFormat = "%02d";

    time -> hour = ((time -> hour + 1) % 24);
    displayHour(dateTimeFormat,*time,isSelected);
}

void incrementMinute(systemTime* time, int isSelected){
    char* dateTimeFormat = "%02d";

    time -> minute = ((time -> minute + 1) % 60);
    displayMinute(dateTimeFormat,*time,isSelected);
}

void incrementSecond(systemTime* time, int isSelected){
    char* dateTimeFormat = "%02d";

    time -> second = ((time -> second + 1) % 60);;
    displaySecond(dateTimeFormat,*time,isSelected);
}




