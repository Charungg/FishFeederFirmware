#ifndef TIME_H
#define TIME_H

typedef struct time{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;

}systemDateTime;

void displayDateBox(char* text);
void displayTimeBox(char* text);


void displayHour(char* format, systemDateTime time, int isSelected);
void displayMinute(char* format, systemDateTime time, int isSelected);
void displaySecond(char* format, systemDateTime time, int isSelected);

void displaySystemTime(char* format, int time, int isSelected, int width, int height);
systemDateTime getSystemDateTime();
void displaySave(int isSelected);
void displaySelected();

void incrementHour(systemDateTime* time);
void incrementMinute(systemDateTime* time);
void incrementSecond(systemDateTime* time);

void isTimeSelected(int select);



///////////////////////////////////////




void runningSetupTime(char* result);
void displaySetupTime(systemDateTime time);

void displaySystemDateTime(systemDateTime time);

void displayDay(char* dayFormat, systemDateTime time, int isSelected);
void displayMonth(char* dayFormat, systemDateTime time, int isSelected);
void displayYear(char* dayFormat, systemDateTime time, int isSelected);

int selectDateTimeOption(int currentOption, systemDateTime* time);
void changingDateTimeValue(void (*incrementFunction)(systemDateTime*),systemDateTime* time);

void incrementDay(systemDateTime* time);
void incrementMonth(systemDateTime* timed);
void incrementYear(systemDateTime* time);

void clearDisplaySelected();

int navigateSetupTime(int currentOption, systemDateTime time);

#endif // TIME_H
