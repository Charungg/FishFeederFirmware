#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "systemTime.h"
#include "menu.h"
#include "dateTimeStruct.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80

void displayDateBox(){
    displayColour("white", "grey");
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*2,"Date |  /  /",1);
}

void displayTimeBox(){
    displayColour("white", "grey");
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*4,"Time |  :  :  ",1);
}


void displayDay(const systemDate* date, int isSelected){
    isTimeSelected(isSelected);
    displayDateTimeText("%02d", date -> day,8,2);
}


void displayMonth(const systemDate* date, int isSelected){
    isTimeSelected(isSelected);
    displayDateTimeText("%02d", date -> month,11,2);
}


void displayYear(const systemDate* date, int isSelected){
    isTimeSelected(isSelected);
    displayDateTimeText("%04d", date -> year,14,2);
}


void displayHour(const systemTime* time, int isSelected){
    isTimeSelected(isSelected);
    displayDateTimeText("%02d", time -> hour,8,4);
}


void displayMinute(const systemTime* time, int isSelected){
    isTimeSelected(isSelected);
    displayDateTimeText("%02d", time -> minute,11,4);
}


void displaySecond(const systemTime* time, int isSelected){
    isTimeSelected(isSelected);
    displayDateTimeText("%02d", time -> second,14,4);
}


void displayDateTimeText(const char* format, int dateTime, int width, int height){
    char text[LINE_SIZE];
    snprintf(text, LINE_SIZE, format, dateTime);
    displayText(CHAR_WIDTH*width,CHAR_HEIGHT*height,text,1);
}


systemDateTime* getSystemDateTime(){
    systemDateTime* newSystemDateTime;
    newSystemDateTime = (systemDateTime*)malloc(sizeof(systemDateTime));

    newSystemDateTime -> date = getSystemDate();
    newSystemDateTime -> time = getSystemTime();

    return newSystemDateTime;
}

systemDate* getSystemDate(){
    systemDate* newSystemDate;
    newSystemDate = (systemDate*)malloc(sizeof(systemDate));

    newSystemDate -> day = clockDay();
    newSystemDate -> month = clockMonth();
    newSystemDate -> year = clockYear();

    return newSystemDate;
}

systemTime* getSystemTime(){
    systemTime* newSystemTime;
    newSystemTime = (systemTime*)malloc(sizeof(systemTime));

    newSystemTime -> hour = clockHour();
    newSystemTime -> minute = clockMinute();
    newSystemTime -> second = clockSecond();

    return newSystemTime;
}


void isTimeSelected(int isSelect){
    if (isSelect){
        displayColour("grey", "white");
    }

    else{
        displayColour("white", "grey");
    }
}


void displaySelected(int isSelected){
    if (isSelected){
        displayColour("white", "black");
    }
    else{
        displayColour("black", "black");
    }
    displayText(0,0,"Selected",1);
}


void incrementDay(systemDateTime* dateTime){
    systemDate* date = dateTime->date;
    int maximumDayOfMonths[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    if ((date -> year) % 4 == 0){
        maximumDayOfMonths[1] = 29;
    }

    date -> day++;

    if ((date -> day) > maximumDayOfMonths[(date -> month) -1]){
        date -> day = 1;
    }

    displayDay(date,1);
}


void incrementMonth(systemDateTime* dateTime){
    systemDate* date = dateTime->date;

    date -> month++;
    if ((date -> month) == 13){
        date -> month = 1;
    }

    displayMonth(date,1);
}


void incrementYear(systemDateTime* dateTime){
    systemDate* date = dateTime->date;

    date -> year = (((date -> year) + 1) % 9999);

    displayYear(date,1);
}


void incrementHour(systemDateTime* dateTime){
    systemTime* time = dateTime->time;

    time -> hour = ((time -> hour + 1) % 25);

    displayHour(time,1);
}


void incrementMinute(systemDateTime* dateTime){
    systemTime* time = dateTime->time;

    time -> minute = ((time -> minute + 1) % 60);

    displayMinute(time,1);
}


void incrementSecond(systemDateTime* dateTime){
    systemTime* time = dateTime->time;

    time -> second = ((time -> second + 1) % 60);;

    displaySecond(time,1);
}


void setClock(systemDateTime* dateTime){
    systemDate* date = dateTime->date;
    systemTime* time = dateTime->time;

    clockSet(time -> second, time-> minute, time-> hour, date->day, date -> month, date -> year);
}





