//
// Created by Charlie Cheung on 13/12/2024.
//

#ifndef FEEDERSCHEDULE_H
#define FEEDERSCHEDULE_H

void addSchedule(feederSystem* feeder, feedingSchedule* newSchedule);
void sortSchedule(feederSystem* feedingScheduleData);

void updateNextFeedTimeIndex(feederSystem* feeder);

int isScheduleBeforeOtherSchedule(feedingSchedule* schedule, feedingSchedule* otherSchedule);
int isScheduleADuplicate(feederSystem* feeder, feedingSchedule* schedule);
int isScheduleTheSame(feedingSchedule* schedule, feedingSchedule* otherSchedule);


#endif //FEEDERSCHEDULE_H
