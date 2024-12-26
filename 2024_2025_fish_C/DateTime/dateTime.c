#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "dateTime.h"
#include "dateTimeStruct.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define LINE_SIZE 80


/**
 * Used to display date time box outline and format.
 */
void displayDateBox(){
    // Set the date box outline and format colour.
    displayColour("white", "grey");

    // Display the box and outline on the JavaFX GUI
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*2,"Date |  /  /",1);
}


/**
 * Used to display the time box outline and format.
 */
void displayTimeBox(){
    // Set the time box outline and format colour.
    displayColour("white", "grey");

    // Display the box and outline on the JavaFX GUI
    displayText(CHAR_WIDTH*2,CHAR_HEIGHT*4,"Time |  :  :  ",1);
}


/**
 * Used to display the day value within the date box.
 * @param date contains day, month and year values.
 * @param isSelected check whether the user is selecting over the day part of the date box.
 */
void displayDay(const systemDate* date, int isSelected){
    // Sets the colour of the day text, depending on if the user is selecting over it.
    isTimeSelected(isSelected);

    // Display day value on the GUI.
    displayDateTimeText("%02d", date -> day,8,2);
}


/**
 * Used to display the month value within the date box.
 * @param date contains day, month and year values.
 * @param isSelected check whether the user is selecting over the day part of the date box.
 */
void displayMonth(const systemDate* date, int isSelected){
    // Sets the colour of the month text, depending on if the user is selecting over it.
    isTimeSelected(isSelected);

    // Display month value on the GUI.
    displayDateTimeText("%02d", date -> month,11,2);
}


/**
 * Used to display the year value within the date box.
 * @param date contains day, month and year values.
 * @param isSelected check whether the user is selecting over the day part of the date box.
 */
void displayYear(const systemDate* date, int isSelected){
    // Sets the colour of the year text, depending on if the user is selecting over it.
    isTimeSelected(isSelected);

    // Display year value on the GUI.
    displayDateTimeText("%04d", date -> year,14,2);
}


/**
 * Used to display the hour value within the time box.
 * @param time contains hour, minute and second values
 * @param isSelected check whether the user is selecting over the day part of the time box.
 */
void displayHour(const systemTime* time, int isSelected){
    // Sets the colour of the hour text, depending on if the user is selecting over it.
    isTimeSelected(isSelected);

    // Display the hour value on the GUI
    displayDateTimeText("%02d", time -> hour,8,4);
}


/**
 * Used to display the minute value within the time box.
 * @param time contains hour, minute and second values
 * @param isSelected check whether the user is selecting over the day part of the time box.
 */
void displayMinute(const systemTime* time, int isSelected){
    // Sets the colour of the minute text, depending on if the user is selecting over it.
    isTimeSelected(isSelected);

    // Display the minute value on the GUI
    displayDateTimeText("%02d", time -> minute,11,4);
}


/**
 * Used to display the second value within the time box.
 * @param time contains hour, minute and second values
 * @param isSelected check whether the user is selecting over the day part of the time box.
 */
void displaySecond(const systemTime* time, int isSelected){
    // Sets the colour of the second text, depending on if the user is selecting over it.
    isTimeSelected(isSelected);

    // Display the second value on the GUI
    displayDateTimeText("%02d", time -> second,14,4);
}


/**
 * Used to display a date or time values to the GUI.
 * @param format is a format specifier to specify how to display the values.
 * @param dateTime is a value from either date or time.
 * @param width specifiers the x-axis where to place the text.
 * @param height specifiers the y-axis where to place the text.
 */
void displayDateTimeText(const char* format, int dateTime, int width, int height){
    char text[LINE_SIZE];
    // Convert the dateTime int value to string.
    snprintf(text, LINE_SIZE, format, dateTime);

    // Display the date time text.
    displayText(CHAR_WIDTH*width,CHAR_HEIGHT*height,text,1);
}


/**
 * Create a systemDateTime struct which holds the date and time values of the system.
 * @return a struct with date and time values of the system.
 */
systemDateTime* getSystemDateTime(){
    systemDateTime* newSystemDateTime;
    // Allocate the struct to the heap.
    newSystemDateTime = (systemDateTime*)malloc(sizeof(systemDateTime));

    // Get the date value of the system.
    newSystemDateTime -> date = getSystemDate();

    // Get the time value of the system.
    newSystemDateTime -> time = getSystemTime();

    return newSystemDateTime;
}


/**
 * Create a date struct which contains the day, month and year values of the system.
 * @return a struct with date values of the system.
 */
systemDate* getSystemDate(){
    systemDate* newSystemDate;
    newSystemDate = (systemDate*)malloc(sizeof(systemDate));

    // Get the day, month and year values from the system.
    newSystemDate -> day = clockDay();
    newSystemDate -> month = clockMonth();
    newSystemDate -> year = clockYear();

    return newSystemDate;
}


/**
 * Create a time struct which contains the hour, minute and second values of the system.
 * @return a struct with time values of the system.
 */
systemTime* getSystemTime(){
    systemTime* newSystemTime;
    newSystemTime = (systemTime*)malloc(sizeof(systemTime*));

    // Get the hour, minute and second values from the system.
    newSystemTime -> hour = clockHour();
    newSystemTime -> minute = clockMinute();
    newSystemTime -> second = clockSecond();

    return newSystemTime;
}


/**
 * Set the colour of the date and time values or boxes, depending on whether the user is selecting over it.
 * Ensures that the user knows which text they are currently selecting.
 * @param isSelect is a boolean to check if the date or time is currently selected.
 */
void isTimeSelected(int isSelect){
    if (isSelect){
        displayColour("grey", "white");
    }

    else{
        displayColour("white", "grey");
    }
}


/**
 * Display a "select" text to indicate values are getting changed by the user.
 * @param isSelected is a boolean .
 */
void displaySelected(int isSelected){
    if (isSelected){
        displayColour("white", "black");
    }

    // Hide the "select" text if the user is no longer changing values.
    else{
        displayColour("black", "black");
    }

    // Displays the "select" text on the top left of the GUI
    displayText(0,0,"Selected",1);
}


/**
 * Increment the day value within the dateTime struct.
 * @param dateTime contains the day value that will be incremented.
 */
void incrementDay(systemDateTime* dateTime){
    systemDate* date = dateTime -> date;

    // Contains the day values of every month (non leap year).
    int maximumDayOfMonths[] = {31,28,31,30,31,30,31,31,30,31,30,31};

    // Checks if the current year is a leap year.
    if ((date -> year) % 4 == 0){
        // Change the day array to contain the leap year maximum day.
        maximumDayOfMonths[1] = 29;
    }

    date -> day++;

    // If the day exceed the maximum day of that month then reset it back to zero.
    if ((date -> day) > maximumDayOfMonths[(date -> month) -1]){
        date -> day = 1;
    }

    // Display the incremented day.
    displayDay(date,1);
}


/**
 * Increment the month value within the dateTime struct.
 * @param dateTime contains the month value that will be incremented.
 */
void incrementMonth(systemDateTime* dateTime){
    systemDate* date = dateTime -> date;
    date -> month++;

    // Ensures the month will always be in the range of 1-12.
    if ((date -> month) > 13){
        date -> month = 1;
    }

    // Display the incremented month.
    displayMonth(date,1);
}


/**
 * Increment the year value within the dateTime struct.
 * @param dateTime contains the year value that will be incremented.
 */
void incrementYear(systemDateTime* dateTime){
    systemDate* date = dateTime -> date;

    // Ensures the year value will always be in the range of 0-9999.
    // Limited to 9999 to fit the format specifier %02d.
    date -> year = (((date -> year) + 1) % 9999);

    // Display the incremented year.
    displayYear(date,1);
}


/**
 * Increment the hour value within the dateTime struct.
 * @param dateTime contains the hour value that will be incremented.
 */
void incrementHour(systemDateTime* dateTime){
    systemTime* time = dateTime -> time;

    // Ensure the hour valid will always be in the range of 0-23.
    time -> hour = ((time -> hour + 1) % 24);

    // Display the incremented hour.
    displayHour(time,1);
}


/**
 * Increment the minute value within the dateTime struct.
 * @param dateTime contains the minute value that will be incremented.
 */
void incrementMinute(systemDateTime* dateTime){
    systemTime* time = dateTime -> time;

    // Ensures the minute value will always be in the range of 0-59.
    time -> minute = ((time -> minute + 1) % 60);

    // Display the incremented minute.
    displayMinute(time,1);
}


/**
 * Increment the second value within the dateTime struct.
 * @param dateTime contains the second value that will be incremented.
 */
void incrementSecond(systemDateTime* dateTime){
    systemTime* time = dateTime -> time;

    // Ensures the second value be in range of 0-59.
    time -> second = ((time -> second + 1) % 60);;

    // Display the incremented second.
    displaySecond(time,1);
}





