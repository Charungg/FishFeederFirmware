#pragma once
// typedef struct time {
//     int day;
//     int month;
//     int year;
//     int hour;
//     int minute;
//     int second;
// }systemDateTime;

typedef struct date{
    int day;
    int month;
    int year;
}systemDate;

typedef struct systemTime{
    int hour;
    int minute;
    int second;
}systemTime;

typedef struct dateAndTime{
    systemDate* date;
    systemTime* time;
}systemDateTime;


