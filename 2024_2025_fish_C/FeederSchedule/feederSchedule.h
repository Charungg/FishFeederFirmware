//
// Created by Charlie Cheung on 13/12/2024.
//

#ifndef FEEDERSCHEDULE_H
#define FEEDERSCHEDULE_H

// User created feeding schedules will be added to the feeder system.
void addSchedule(feederSystem* feeder, feedingSchedule* newSchedule);

// Sort the feeder system array of feeding schedules from Monday - Sunday.
// With the use of insertion sort.
void sortSchedule(const feederSystem* feeder);

// Find the next feed time schedules.
void updateNextFeedTimeIndex(feederSystem* feeder);

// Checks if the schedules is before the other schedules.
int isScheduleBeforeOtherSchedule(feedingSchedule* schedule, feedingSchedule* otherSchedule);

// Checks if the user created feeding schedules already exist within the feeder system.
int isScheduleADuplicate(const feederSystem* feeder, feedingSchedule* newSchedule);

// Checks if two schedules are the same in terms of time and number of feed data.
int isScheduleTheSame(feedingSchedule* schedule, feedingSchedule* otherSchedule);

#endif //FEEDERSCHEDULE_H
