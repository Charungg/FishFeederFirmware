#ifndef TIME_H
#define TIME_H
#include "timeStruct.h"

void displayDateBox(char* text);
void displayTimeBox(char* text);


void displayHour(char* format, systemDateTime time, int isSelected);
void displayMinute(char* format, systemDateTime time, int isSelected);
void displaySecond(char* format, systemDateTime time, int isSelected);

void displaySystemTime(const char* format, int time, int isSelected, int width, int height);
systemDateTime getSystemDateTime();
void displaySave(int isSelected);
void displaySelected();

void incrementHour(systemDateTime* time);
void incrementMinute(systemDateTime* time);
void incrementSecond(systemDateTime* time);

void isTimeSelected(int select);

#endif // TIME_H
