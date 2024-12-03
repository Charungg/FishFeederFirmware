#include <stdio.h>

#include "fish.h"
#include "time.h"
#include "timeStruct.h"
#include "menu.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

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

void displaySystemTime(const char* format, int time, int isSelected, int width, int height){
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




