//
// Created by Charlie Cheung on 09/12/2024.
//

#ifndef FEEDINGSCHEDULESTRUCT_H
#define FEEDINGSCHEDULESTRUCT_H
#include "dateTimeStruct.h"

typedef struct{
    int dayOfWeek;
    int numberOfFeed;
    systemTime* time;
}feedingSchedule;


typedef struct{
    int nextFeedTimeIndex;              // Holds the index of the next feed time.
    int totalNumberOfFeed;              // Holds the total number of feed.
    char operatingMode;                 // A = Auto, P = Paused and S = Skip Next Feed.
    int amountOfSchedule;               // Holds the amount of schedule within the scheduleData pointer
    feedingSchedule* arrayOfSchedules;   // Pointer to where the array of schedule will be on the heap.
}feedingScheduleData;

#endif //FEEDINGSCHEDULESTRUCT_H
