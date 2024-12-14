#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "dateTime.h"
#include "feedingScheduleStruct.h"
#include "feederSchedule.h"


#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define LINE_SIZE 80


// Adds the user created schedule to the array of schedules
void addSchedule(feederSystem* feeder, feedingSchedule* newSchedule){
    printf("---------------------------------------------------------\n");

    // Ensures the added schedules is an already existing schedules within the array of schedules.
    if (isScheduleADuplicate(feeder, newSchedule)){
        return;
    }

    // To instantiate the array of schedules pointer when there is no schedules at first.
    if (feeder -> amountOfSchedules == 0){
        feeder -> arrayOfSchedules = (feedingSchedule*)malloc(sizeof(feedingSchedule));
        feeder -> nextFeedTimeIndex = 0;
    }

    // Dynamically resize the arrayOfSchedules with the use of realloc().
    else{
        feeder -> arrayOfSchedules = realloc(feeder -> arrayOfSchedules, sizeof(feedingSchedule)*(feeder -> amountOfSchedules + 1));
    }

    // Store the user created schedules to the array of schedules.
    feeder -> arrayOfSchedules[feeder -> amountOfSchedules] = *newSchedule;
    // Increment the feeder struct that stores the amount schedules within the array pointer.
    feeder -> amountOfSchedules += 1;

    // Sorts the array of schedules from Monday - Sunday, Time 00:00:00 - 24:00:00
    sortSchedule(feeder);
    // Updates the nextFeedTimeIndex which points to a schedules within the array so that the system knows if there is any schedules happening.
    updateNextFeedTimeIndex(feeder);

    printf("---------------------------------------------------------\n");
}


// Sorts the array of schedules from Monday - Sunday.
void sortSchedule(feederSystem* feedingScheduleData){
    feedingSchedule* scheduleArray = feedingScheduleData -> arrayOfSchedules;
    int sizeOfArray = feedingScheduleData -> amountOfSchedules;

    // Insertion Sorting Algorithm.
    // From index 1 it will keep incrementing till the amount of schedules.
    for (int i = 1; i<sizeOfArray; i++){
        // Grabs the element corresponding from index i.
        feedingSchedule schedule = scheduleArray[i];
        // J is will be used to grab schedules behind index i.
        int j = i - 1;

        // Checks if schedules of J index is not before schedules of index i.
        while(j >= 0 && !isScheduleBeforeOtherSchedule(&scheduleArray[j], &schedule)){
            // Swapping process of index J and index i elements.
            scheduleArray[j + 1] = scheduleArray[j];
            // Keeps backtracking till the original index i element is in correct order.
            j = j - 1;
        }
        scheduleArray[j + 1] = schedule;
    }
}


// Updates the nextFeedTimeIndex in feeder so the checkSchedules() knows which schedules within the array is upcoming for feedingTime().
void updateNextFeedTimeIndex(feederSystem* feeder){
    // Grabs the current systemTime and dayOfWeek
    feedingSchedule currentSystemTime = {(clockDayOfWeek() + 6)%7, 0, clockHour(), clockMinute(), clockSecond()};
    int nextFeedTimeFound = 0;
    int amountOfSchedules = feeder -> amountOfSchedules;

    printf("\n");
    printf("Current System Time | DayOfWeek: %d | Time: %02d,%02d,%02d\n", currentSystemTime.dayOfWeek, currentSystemTime.time.hour, currentSystemTime.time.minute, currentSystemTime.time.second);
    printf("amount of schedules: %d\n", feeder -> amountOfSchedules);
    printf("\n");

    for (int i = 0; i<amountOfSchedules; i++){
        feedingSchedule* schedule = &feeder -> arrayOfSchedules[i];
        printf("| Schedule[%d] | DayOfWeek: %d | Time: %02d,%02d,%02d\n", i, schedule->dayOfWeek, schedule->time.hour, schedule->time.minute, schedule->time.second);
    }

    // Iterates through the array of schedules.
    for (int i=0; i<amountOfSchedules; i++){
        // Checks if the systemTime is before any schedules
        if (isScheduleBeforeOtherSchedule(&currentSystemTime, &feeder->arrayOfSchedules[i])){
            feeder -> nextFeedTimeIndex = i;
            nextFeedTimeFound = 1;
            break;
        }
    }

    if (!nextFeedTimeFound){
        feeder -> nextFeedTimeIndex = 0;
    }
}


// Couldn't think of an easier understanding function name but the idea of this function is to check whether a given schedule comes first before other schedule.
// For example one O clock happens before six O clock, Monday happens before Wednesday happens etc.
// By having this function I can arrange my array of schedule from smallest to largest and check whether a given schedule is after or before the systemTime.
int isScheduleBeforeOtherSchedule(feedingSchedule* schedule, feedingSchedule* otherSchedule){
    const int dataSet = 4;
    systemTime* scheduleTime = &schedule -> time;
    systemTime* otherScheduleTime = &otherSchedule -> time;

    int scheduleData[] = {schedule -> dayOfWeek, scheduleTime -> hour, scheduleTime -> minute, scheduleTime -> second};

    int otherScheduleData[] = {otherSchedule -> dayOfWeek, otherScheduleTime -> hour, otherScheduleTime -> minute, otherScheduleTime -> second};

    printf("Comparing Schedule: %d, %02d:%02d:%02d With Other Schedule : %d, %02d:%02d:%02d\n", scheduleData[0],scheduleData[1],scheduleData[2],scheduleData[3],otherScheduleData[0],otherScheduleData[1],otherScheduleData[2],otherScheduleData[3]);
    for (int i = 0; i < dataSet; i++){
        if (scheduleData[i] < otherScheduleData[i]){
            return 1;
        }

        if(scheduleData[i] > otherScheduleData[i]){
            return 0;
        }
    }

    // Edge case where the systemTime is the same as the feeder time schedule.
    // Has to return zero else the feeder next feed time will be the same time next week to feed the fish.
    // That would not be an ideal situation
    return 0;
}


// Checks if the schedule Already exist within the feeder schedule array.
int isScheduleADuplicate(feederSystem* feeder, feedingSchedule* schedule){

    // Iterates through every schedule within the feeder schedule array.
    for (int i=0; i<feeder->amountOfSchedules; i++){
        // Checks if the schedule is identical to any schedule within the feeder schedule array.
        if (isScheduleTheSame(schedule, &feeder->arrayOfSchedules[i])){
            // Duplicate schedule found
            return 1;
        }
    }

    // No duplicate schedule
    return 0;
}


// Checks if the two given schedules are same
int isScheduleTheSame(feedingSchedule* schedule, feedingSchedule* otherSchedule){
    systemTime* scheduleTime = &schedule -> time;
    systemTime* otherScheduleTime = &schedule -> time;
    const int scheduleDataSize = 4;

    // Store all schedule dayOfWeek and systemTimes within their own separate arrays.
    int scheduleData[] = {schedule -> dayOfWeek, scheduleTime -> hour, scheduleTime -> minute, scheduleTime -> second};
    int otherScheduleData[] = {otherSchedule -> dayOfWeek, otherScheduleTime -> hour, otherScheduleTime -> minute, otherScheduleTime -> second};

    // Iterates through each schedule data
    for (int i = 0; i < scheduleDataSize; i++){

        // Checks if the values corresponding to their value are not the same
        if (scheduleData[i] != otherScheduleData[i]){
            // A schedule values are not the same.
            return 0;
        }
    }

    // All values are the same.
    return 1;
}