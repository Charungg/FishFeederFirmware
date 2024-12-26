#ifndef TIME_H
#define TIME_H
#include "dateTimeStruct.h"

// Display the boxes around the date and time texts.
void displayDateBox();
void displayTimeBox();

// Display the system time values on the GUI.
void displayDay(const systemDate* date, int isSelected);
void displayMonth(const systemDate* date, int isSelected);
void displayYear(const systemDate* date, int isSelected);

// Display the system date values on the GUI.
void displayHour(const systemTime* time, int isSelected);
void displayMinute(const systemTime* time, int isSelected);
void displaySecond(const systemTime* time, int isSelected);

// Displays the date or time values on the GUI
void displayDateTimeText(const char* format, int dateTime, int width, int height);

// Get system date and/or time structs.
systemDateTime* getSystemDateTime();
systemDate* getSystemDate();
systemTime* getSystemTime();

// Display a select text to indicate user changing values.
void displaySelected(int isSelected);

// Increment the day, month and year values from date struct.
void incrementDay(systemDateTime* dateTime);
void incrementMonth(systemDateTime* dateTime);
void incrementYear(systemDateTime* dateTime);

// Increment the hour, minute and second values from time struct.
void incrementHour(systemDateTime* dateTime);
void incrementMinute(systemDateTime* dateTime);
void incrementSecond(systemDateTime* dateTime);

// Changes the date or time colours depending on if the user is selecting them.
void isTimeSelected(int isSelect);
#endif // TIME_H
