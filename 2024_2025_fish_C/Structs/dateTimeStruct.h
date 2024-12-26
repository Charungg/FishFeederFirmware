#pragma once

/**
 * Struct to date values.
 */
typedef struct{
    int day;
    int month;
    int year;
}systemDate;


/**
 * Struct to time values.
 */
typedef struct{
    int hour;
    int minute;
    int second;
}systemTime;


/**
 * Struct to hold pointers to both date and time values.
 */
typedef struct{
    systemDate* date;
    systemTime* time;
}systemDateTime;




