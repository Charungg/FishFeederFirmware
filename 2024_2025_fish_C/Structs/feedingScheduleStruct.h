//
// Created by Charlie Cheung on 09/12/2024.
//

#ifndef FEEDINGSCHEDULESTRUCT_H
#define FEEDINGSCHEDULESTRUCT_H
#include "dateTimeStruct.h"
#include "operatingModeEnum.h"

/**
 * Struct to hold each individuals feeding schedules.
 */
typedef struct{
    int dayOfWeek;      // Every schedule is specific to a day of the week.
    int numberOfFeed;   // Each schedule want to fish the fish a certain amount of time
    systemTime time;    // Each feeding session can only happen at a certain time.
}feedingSchedule;


/**
 * Struct to store feeder system data such as next feed time index of array of schedules.
 * Total number of feed, amount of feeding schedules and operating mode.
 */
typedef struct{
    int nextFeedTimeIndex;              // Holds the index of the next feed time.
    int totalNumberOfFeed;              // Holds the total number of feed.
    operatingEnum operatingMode;                 // A = Auto, P = Paused and S = Skip Next Feed.
    int amountOfSchedules;               // Holds the amount of schedule within the scheduleData pointer
    feedingSchedule* arrayOfSchedules;   // Pointer to where the array of schedule will be on the heap.
}feederSystem;

#endif //FEEDINGSCHEDULESTRUCT_H
