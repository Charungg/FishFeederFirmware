#ifndef TIME_H
#define TIME_H
#include "dateTimeStruct.h"

void displayDateBox();
void displayTimeBox();

void displayDay(const systemDate* date, int isSelected);
void displayMonth(const systemDate* date, int isSelected);
void displayYear(const systemDate* date, int isSelected);

void displayHour(const systemTime* time, int isSelected);
void displayMinute(const systemTime* time, int isSelected);
void displaySecond(const systemTime* time, int isSelected);

void displayDateTimeText(const char* format, int dateTime, int width, int height);

systemDateTime* getSystemDateTime();
systemDate* getSystemDate();
systemTime* getSystemTime();


void displaySelected(int isSelected);

void incrementDay(systemDateTime* dateTime);
void incrementMonth(systemDateTime* dateTime);
void incrementYear(systemDateTime* dateTime);

void incrementHour(systemDateTime* dateTime);
void incrementMinute(systemDateTime* dateTime);
void incrementSecond(systemDateTime* dateTime);

void isTimeSelected(int isSelect);

void setClock(systemDateTime* dateTime);

#endif // TIME_H
