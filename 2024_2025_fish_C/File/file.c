// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
// #include "fish.h"
// #include "file.h"
//
// #include "setupScheduleTime.h"
//
// #define LINE_SIZE 80
//
// extern scheduleData feederSchedule[];
//
// void saveSystemDateTime(systemDateTime* systemDateTime) {
//     if(isScheduleTimeSaveExist() == 0){
//         printf("saveSystemToTime Option 1\n");
//         writeSystemDateTimeToFile(systemDateTime);
//     }
//
//     else{
//         printf("saveSystemToTime Option 2\n");
//         scheduleData* schedule = getScheduleDataFromFile();;
//
//         // systemTime* scheduleTime = scheduleData ->dateTime -> time;
//         //
//         // for(int i = 0; i < amountOfSchedules; i++){
//         //     printf("%d | %02d:%02d:%02d | %02d", scheduleData->dayOfWeek, scheduleTime->hour, scheduleTime->minute, scheduleTime->second, scheduleData -> numberOfFeed);
//         // }
//         // Write Time
//         // Write Schedule
//         free(schedule);
//     }
// }
//
//
// void writeSystemDateTimeToFile(systemDateTime* systemDateTime){
//     FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "w");
//     systemDate* date = systemDateTime -> date;
//     systemTime* time = systemDateTime -> time;
//
//     printf("writeSystemDateTimeToFile");
//
//     fprintf(filePtr, "%s\n", "systemDateTime:");
//     fprintf(filePtr, "%02d/%02d/%04d %02d:%02d:%02d\n", date -> day, date -> month, date -> year, time -> hour, time -> minute, time -> second);
//     fclose(filePtr);
// }
//
//
// void loadSystemDateTimeSave() {
//     if (isSystemTimeSaveExist() == 1) {
//         printf("READING\n");
//         systemDateTime* dateTime = getSystemDateTimeFromFile();
//
//         /// malloc worked last time but grayed out
//         printf("READING Done\n");
//
//         printf("loadSystemDateTimeSave\n");
//         printf("Date = %02d/%02d/%04d\n",dateTime->date->day, dateTime->date->month, dateTime->date->year);
//         printf("Time = %02d:%02d:%02d\n", dateTime->time->hour, dateTime-> time->minute, dateTime->time->second);
//
//         loadClock(dateTime);
//     }
//
//     else{
//         clockSet(0, 30, 23, 30, 12, 2004);
//     }
// }
//
// systemDateTime* getSystemDateTimeFromFile() {
//     char lineText[LINE_SIZE];
//     char* dateString;
//     char* timeString;
//     FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");
//
//     systemDateTime* dateTime = malloc(sizeof(systemDateTime));
//     getNextLineText(lineText,filePtr,2);
//
//     dateString = strtok(lineText, " ");
//     timeString = strtok(NULL, " ");
//
//     dateTime -> date = dateStringToSystemDate(dateString);
//     dateTime -> time = timeStringToSystemTime(timeString);
//
//     // printf("SystemDate = %02d:%02d:%04d\n", dateTime->date->day, dateTime->date->month, dateTime->date->year);
//     // printf("SystemTime = %02d:%02d:%02d\n", dateTime->time->hour, dateTime->time->minute, dateTime->time->second);
//     //
//     // free(dateString);
//     // free(timeString);
//     fclose(filePtr);
//
//     return dateTime;
// }
//
//
// systemDate* dateStringToSystemDate(char* dateString){
//     char* text;
//     int dateValue[3];
//     int index = 0;
//
//     systemDate* newDate = malloc(sizeof(systemDate));
//     text = strtok(dateString, "/");
//
//     do{
//         removeZeroPadding(text);
//
//         dateValue[index] = atoi(text);
//         index++;
//
//         text = strtok(NULL, "/");
//
//     }while(text != NULL);
//
//     newDate -> day = dateValue[0];
//     newDate -> month = dateValue[1];
//     newDate -> year = dateValue[2];
//
//     return newDate;
// }
//
//
// systemTime* timeStringToSystemTime(char* timeString){
//     char* text;
//     int timeValue[3];
//     int index = 0;
//
//     printf("INSIDE TIME STRING: %s\n", timeString);
//     systemTime* newTime = malloc(sizeof(systemTime));
//
//     text = strtok(timeString, ":");
//     printf("INSIDE TEXT: %s\n",text);
//
//     do{
//         removeZeroPadding(text);
//         printf("INSIDE TEXT 2: %s\n",text);
//         timeValue[index] = atoi(text);
//         printf("TIME VALUE[%d]: %d\n", index, timeValue[index]);
//         index++;
//
//         text = strtok(NULL, ":");
//
//     }while(text != NULL);
//
//     newTime->hour = timeValue[0];
//     newTime->minute = timeValue[1];
//     newTime->second = timeValue[2];
//
//     printf("INSIDE NEW TIME =  %d:%d:%d\n",newTime->hour, newTime->minute, newTime->second);
//
//     return newTime;
// }
//
//
//
//
// void saveScheduleTime(scheduleData* scheduleData){
//     if (isSystemTimeSaveExist() == 1 && isScheduleTimeSaveExist() == 0){
//         printf("SAVE SCHEDULE OPTION 1\n");
//         char lineText[LINE_SIZE];
//
//         FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");
//         getNextLineText(lineText,filePtr,2);
//         fclose(filePtr);
//
//         systemDateTime* dateTime = getSystemDateTimeFromFile();;
//         writeSystemDateTimeToFile(dateTime);
//         writeScheduleTimeToFile(scheduleData);
//         free(dateTime);
//     }
//
//     else{
//         printf("SAVE SCHEDULE OPTION 2\n");
//         writeScheduleTimeToFile(scheduleData);
//     }
// }
//
// void writeScheduleTimeToFile(scheduleData* scheduleData){
//     FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "a");
//     systemTime* scheduleTime = scheduleData -> dateTime -> time;
//
//     if (isScheduleTimeSaveExist() == 0){
//         fprintf(filePtr, "%s\n", "scheduleTime:");
//     }
//
//     fprintf(filePtr, "%d %02d:%02d:%02d %02d\n", scheduleData -> dayOfWeek, scheduleTime ->hour, scheduleTime -> minute, scheduleTime -> second, scheduleData -> numberOfFeed );
//     fclose(filePtr);
// }
//
//
//
//
// void loadScheduleTime(){
//     if (isScheduleTimeSaveExist() == 1){
//         scheduleData* schedule = getScheduleDataFromFile();;
//         free(schedule);
//     }
// }
//
//
// scheduleData* getScheduleDataFromFile(){
//     char lineText[LINE_SIZE];
//     int amountOfSchedules = getAmountOfSchedules();
//
//     int dayOfWeek;
//     char* timeString;
//     int numberOfFeed;
//
//     printf("AMOUNT OF SCHEDULES: %d\n", amountOfSchedules);
//     scheduleData* schedule = malloc(amountOfSchedules * sizeof(*schedule));
//
//     FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");
//
//     if (isSystemTimeSaveExist() == 1){
//         getNextLineText(lineText,filePtr,3);
//     }
//
//     else{
//         getNextLineText(lineText,filePtr,1);
//     }
//
//     for (int i = 0; i<amountOfSchedules; i++){
//         fgets(lineText,LINE_SIZE,filePtr);
//
//         printf("LINE TEXT: %s",lineText);
//
//         dayOfWeek = atoi(strtok(lineText, " "));
//         timeString = strtok(NULL, " ");
//         numberOfFeed = atoi(strtok(NULL, " "));
//
//         printf("DAY OF WEEK: %d\n", dayOfWeek);
//         printf("TIME STRING: %s\n", timeString);
//         printf("NUMBER OF FEEDERS: %d\n", numberOfFeed);
//
//
//         schedule[i].dayOfWeek = dayOfWeek;
//         printf("DAY OF WEEK VALUE: %d\n", schedule[0].dayOfWeek);
//
//
//
//         printf("------------------\n");
//     }
//
//     fclose(filePtr);
//
//     return schedule;
// }
//
//
//
//
// int getAmountOfSchedules(){
//     char lineText[LINE_SIZE];
//     int amountOfSchedules = 0;
//     FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");
//
//     if (filePtr == NULL){
//         printf("Zero Schedules");
//         return 0;
//     }
//
//     if (isSystemTimeSaveExist() == 1){
//         getNextLineText(lineText,filePtr,3);
//     }
//
//     else{
//         getNextLineText(lineText,filePtr,1);
//     }
//
//     while(fgets(lineText, LINE_SIZE, filePtr)){
//         amountOfSchedules++;
//     }
//
//     fclose(filePtr);
//
//     return amountOfSchedules;
// };
//
//
// int isSystemTimeSaveExist(){
//     FILE* filePtr;
//     filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");
//
//     if (filePtr == NULL){
//         return 0;
//     }
//
//     char lineText[LINE_SIZE];
//     while(fgets(lineText, sizeof(lineText), filePtr)){
//         if (strcmp(lineText, "systemDateTime:\n") == 0){
//             return 1;
//         }
//     }
//     fclose(filePtr);
//     return 0;
// }
//
//
// int isScheduleTimeSaveExist(){
//     FILE* filePtr;
//     filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");
//
//     if (filePtr == NULL){
//         return 0;
//     }
//
//     char lineText[LINE_SIZE];
//     while(fgets(lineText, sizeof(lineText), filePtr)){
//         if (strcmp(lineText, "scheduleTime:\n") == 0){
//             return 1;
//         }
//     }
//
//     fclose(filePtr);
//     return 0;
// }
//
//
// void removeZeroPadding(char* value){
//     // n will hold the index after all the zeros padding.
//     int n;
//
//     //Checks through all the element of date or time string by iterating through to find a char that is not
//     //a zero and ensures the zero found is not the null-terminated string literal.
//     if((n = strspn(value, "0")) != 0 && (value[n] != '\0')) {
//
//         //Copies the part of the date or time substring that does not contain the 0 padding by
//         //Overwriting the original string pointer.
//         strcpy(value, &value[n]);
//     }
// }
//
//
// void loadClock(systemDateTime* systemDateTime) {
//     //SEC MIN HOUR DAY MONTH YEAR
//     systemDate* date = systemDateTime -> date;
//     systemTime* time = systemDateTime -> time;
//     clockSet(time -> second, time -> minute, time -> hour, date -> day, date -> month, date -> year);
// }
//
//
// void getNextLineText(char* lineText,FILE* filePtr, int amountOfRead){
//     for (int i = 0; i < amountOfRead; i++){
//         fgets(lineText, LINE_SIZE, filePtr);
//     }
// }
//
//
