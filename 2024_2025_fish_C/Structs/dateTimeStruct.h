#pragma once
typedef struct{
    int day;
    int month;
    int year;
}systemDate;

typedef struct{
    int hour;
    int minute;
    int second;
}systemTime;

typedef struct{
    systemDate* date;
    systemTime* time;
}systemDateTime;




