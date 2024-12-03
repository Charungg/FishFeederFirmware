#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "time.h"
#include "menu.h"
#include "file.h"
#include "userControl.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// 64/8 = 8 HEIGHT
// 128/6 = 21.333^* WIDTH

#define LINE_SIZE 80

void displayDateBox(char* text){
    displayColour("white", "grey");
    text = "Date |  /  /";
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*2,text,1);
}

void displayTimeBox(char* text){
    displayColour("white", "grey");
    text = "Time |  :  :  ";
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*4,text,1);
}


void displayHour(char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.hour, isSelected,8,4);
}

void displayMinute(char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.minute, isSelected,11,4);
}

void displaySecond(char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.second, isSelected,14,4);
}

void displaySystemTime(char* format, int time, int isSelected, int width, int height){
    char text[LINE_SIZE];
    isTimeSelected(isSelected);
    snprintf(text, LINE_SIZE, format, time);
    displayText(CHAR_WIDTH*width,CHAR_HEIGHT*height,text,1);
}


systemDateTime getSystemDateTime(){
    systemDateTime time;
    time.day = clockDay();
    time.month = clockMonth();
    time.year = clockYear();
    time.hour = clockHour();
    time.minute = clockMinute();
    time.second = clockSecond();
    return time;
}

void displaySave(int isSelected) {
    isOptionSelected(isSelected);
    displayText(CHAR_WIDTH*17,CHAR_HEIGHT*7,"Save",1);
}


void isTimeSelected(int select){
    if (select){
        displayColour("grey", "white");
    }

    else{
        displayColour("white", "grey");
    }
}


void displaySelected(){
    displayColour("white", "black");
    displayText(0,0,"Selected",1);
}


void incrementHour(systemDateTime* time){
    char* dateTimeFormat = "%02d";

    time -> hour = ((time -> hour + 1) % 24);
    displayHour(dateTimeFormat,*time,1);
}

void incrementMinute(systemDateTime* time){
    char* dateTimeFormat = "%02d";

    time -> minute = ((time -> minute + 1) % 60);
    displayMinute(dateTimeFormat,*time,1);
}

void incrementSecond(systemDateTime* time){
    char* dateTimeFormat = "%02d";

    time -> second = ((time -> second + 1) % 60);;
    displaySecond(dateTimeFormat,*time,1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void runningSetupTime(char* result) {
    int running_setup_time = 1;
    int currentOption = 0;

    printf("Short press to navigate, long press to change value\n");
    systemDateTime time = getSystemDateTime();

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


void displayDay(char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.day, isSelected,8,2);
}


void displayMonth(char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.month, isSelected,11,2);
}


void displayYear(char* format, systemDateTime time, int isSelected){
    displaySystemTime(format, time.year, isSelected,14,2);
}


int selectDateTimeOption(int currentOption, systemDateTime* time){
    void (*incrementFunction)(systemDateTime*);

    switch(currentOption){
    case 0:
        displaySelected();
        incrementFunction = &incrementDay;
        changingDateTimeValue(incrementFunction, time);
        break;

    case 1:
        displaySelected();
        incrementFunction = &incrementMonth;
        changingDateTimeValue(incrementFunction, time);
        break;

    case 2:
        displaySelected();
        incrementFunction = &incrementYear;
        changingDateTimeValue(incrementFunction, time);
        break;

    case 3:
        displaySelected();
        incrementFunction = &incrementHour;
        changingDateTimeValue(incrementFunction, time);
        break;

    case 4:
        displaySelected();
        incrementFunction = &incrementMinute;
        changingDateTimeValue(incrementFunction, time);
        break;

    case 5:
        displaySelected();
        incrementFunction = &incrementSecond;
        changingDateTimeValue(incrementFunction, time);
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


void changingDateTimeValue(void (*incrementFunction)(systemDateTime*),systemDateTime* time){
    int changing_value = 1;
    char* result;

    do{
        msleep(500L);
        result = buttonState();

        if (isUserAFK(result)){
            displaySetupTime(*time);
        }

        else if (isLongPressed(result)){
            clearDisplaySelected();
            changing_value = 0;
        }

        else if (isShortPressed(result)){
            incrementFunction(time);
        }

    }while(changing_value);
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
    if ((time -> month) % 12 == 0){
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





