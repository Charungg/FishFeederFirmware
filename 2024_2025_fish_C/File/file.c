#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fish.h"
#include "file.h"

#include "dateTime.h"
#include "feederSchedule.h"
#include "scheduleTime.h"

#define LINE_SIZE 80

/**
 * Used to save the user created system date and time to the fish feeder save text file.
 * @param systemDateTime is a struct which contains user created system date and time.
 */
void saveSystemDateTime(systemDateTime* systemDateTime) {
    // If there is no schedules saved within the text file
    if(isScheduleTimeSaveExist() == 0){
        // Write the system date time to the save text file.
        writeSystemDateTimeToFile(systemDateTime);
    }

    else{
        // Get the amount of schedules within the save file.
        int amountOfSchedule = getAmountOfSchedules();

        // Get the schedules data from the save file.
        feedingSchedule* arrayOfSchedules = getScheduleDataFromFile();

        // Write the Date and time to the text file.
        writeSystemDateTimeToFile(systemDateTime);

        // Iterates through the array of schedules.
        for(int i=0; i<amountOfSchedule; i++){
            // Write each schedule that exits within array of schedules to the text file.
            writeScheduleTimeToFile(&arrayOfSchedules[i]);
        }
    }
}


/**
 * Write the user created date and time to the save text file.
 * @param systemDateTime contains the date and time.
 */
void writeSystemDateTimeToFile(const systemDateTime* systemDateTime){
    // Write mode onto the save file.
    FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "w");
    systemDate* date = systemDateTime -> date;
    systemTime* time = systemDateTime -> time;

    // Write the systemDateTime line to indicate save data and then the system date and time.
    fprintf(filePtr, "%s\n", "systemDateTime:");
    fprintf(filePtr, "%02d/%02d/%04d %02d:%02d:%02d\n", date -> day, date -> month, date -> year, time -> hour, time -> minute, time -> second);
    fclose(filePtr);
}


/**
 * Load the saved system date and time from the save text file.
 */
void loadSystemDateTimeSave() {
    // If the system date time save exist.
    if (isSystemDateTimeSaveExist() == 1) {
        // Get the system date time from the save text file.
        systemDateTime dateTime = getSystemDateTimeFromFile();
        // Load the system data time from save to system.
        loadClock(&dateTime);
    }

    else{
        // Load the system to current time if there is no save date time files.
       clockWarmStart(1);
    }
}


/**
 * Get the system date time from save text file.
 * @return a system date time struct which contains values from the save text file.
 */
systemDateTime getSystemDateTimeFromFile() {
    char lineText[LINE_SIZE];
    char* dateString;
    char* timeString;

    // Read mode on the save text file.
    FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");

    systemDateTime dateTime;
    // Get the 2nd line text of the save text file.
    getNextLineText(lineText,filePtr,2);

    // Splits the line text into date and time strings.
    dateString = strtok(lineText, " ");
    timeString = strtok(NULL, " ");

    // Convert date and time string into there respective struct which store
    // values for date and time.
    dateTime.date = dateStringToSystemDate(dateString);
    dateTime.time = timeStringToSystemTime(timeString);

    fclose(filePtr);

    return dateTime;
}


/**
 * Convert the date string to system date struct which holds day, month and year values.
 * @param dateString which contains day, month and year values.
 * @return a converted date string to system date struct.
 */
systemDate* dateStringToSystemDate(char* dateString){
    char* text;
    char* endPtr;
    int dateValue[3];
    int index = 0;

    systemDate* newDate = malloc(sizeof(systemDate));
    // Get the day value of the date string.
    text = strtok(dateString, "/");

    do{
        // Remove the zero padding of the values.
        // e.g. 03/08/2024 -> 3/8/2024.
        removeZeroPadding(text);

        // Convert the string value to an integer.
        dateValue[index] = strtol(text, &endPtr, 10);
        index++;

        // Grab the next value, month or year.
        text = strtok(NULL, "/");

    }while(text != NULL);

    // Pass the converted date string to the system date struct.
    newDate->day = dateValue[0];
    newDate->month = dateValue[1];
    newDate->year = dateValue[2];

    return newDate;
}


/**
 * Convert the time string to system time struct which holds hour, minute and second values
 * @param timeString which contains hour, minute and second values
 * @return a converted time string to system time struct.
 */
systemTime* timeStringToSystemTime(char* timeString){
    char* text;
    char* endPtr;
    int timeValue[3];
    int index = 0;

    systemTime* newTime = malloc(sizeof(systemTime));
    // Get the hour value of the time string.
    text = strtok(timeString, ":");

    do{
        // Remove the zero padding of the values.
        // e.g. 04:55:06 -> 4:55:6
        removeZeroPadding(text);

        // Convert the string value to an integer.
        timeValue[index] = strtol(text, &endPtr, 10);
        index++;

        // Grab the next value, minute and second.
        text = strtok(NULL, ":");

    }while(text != NULL);

    // Pass the converted time string to the system time struct.
    newTime->hour = timeValue[0];
    newTime->minute = timeValue[1];
    newTime->second = timeValue[2];

    return newTime;
}


/**
 * Save the user created feeding schedule to the save text file.
 * @param schedule is the user created feeding schedule.
 */
void saveScheduleTime(feedingSchedule* schedule){

    // If the there is system date time exist and schedule save text file does not exist.
    if (isSystemDateTimeSaveExist() == 1 && isScheduleTimeSaveExist() == 0){

        // Get the system date time save from the text file.
        systemDateTime dateTime = getSystemDateTimeFromFile();;
        // Write the system date time to save text file.
        writeSystemDateTimeToFile(&dateTime);
        // Write the feeding schedule to save text file.
        writeScheduleTimeToFile(schedule);
    }

    else{
        // Write the feeding schedule to save text file.
        writeScheduleTimeToFile(schedule);
    }
}


/**
 * Write the user created feeding schedule to the save text file.
 * @param schedule is the user created feeding schedule for the fishes.
 */
void writeScheduleTimeToFile(feedingSchedule* schedule){
    // Append mode to save text file.
    FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "a");
    systemTime* scheduleTime = &schedule-> time;

    // If there is no schedule save indicator.
    // To allow the read part of the code to determine if there is any schedule in the first place.
    if (isScheduleTimeSaveExist() == 0){
        fprintf(filePtr, "%s\n", "scheduleTime:");
    }

    // Append the feeding schedule data to the save text file.
    fprintf(filePtr, "%d %02d:%02d:%02d %02d\n", schedule -> dayOfWeek, scheduleTime ->hour, scheduleTime -> minute, scheduleTime -> second, schedule -> numberOfFeed );
    fclose(filePtr);
}


/**
 * Load the feeder system by converting all feeding schedules from save text file to the feeder system.
 * @return all feeding schedules data from save text file.
 */
feederSystem* loadFeederSystem(){
    feederSystem* loadingFeederSystem = malloc(sizeof(feederSystem));

    // By default, the feeder system operating mode will be set to auto.
    // Total number of feed to zero.
    // Since the schedules saved to the text file are not in order the next feed time index will be set to -1.
    // An array of schedules pointer does not know how big the array is so the getAmountOfSchedules()
    // will help determine how many schedules there is in the array.
    loadingFeederSystem -> nextFeedTimeIndex = -1;
    loadingFeederSystem -> totalNumberOfFeed = 0;
    loadingFeederSystem -> operatingMode = AUTO;
    loadingFeederSystem -> amountOfSchedules = getAmountOfSchedules();

    // If there is no saved schedule in the text file.
    if (isScheduleTimeSaveExist() == 0){
        // Set up a default feeding schedules for new users.
        loadingFeederSystem -> arrayOfSchedules = setupFeedingSchedule();
    }

    else{
        // Load the user created feeding schedules from the save text file.
        loadingFeederSystem -> arrayOfSchedules = getScheduleDataFromFile();

        // Sort the feeder system array of schedules from Monday - Sunday order.
        sortSchedule(loadingFeederSystem);

        // Then determine which schedules is up next on the feeder system.
        updateNextFeedTimeIndex(loadingFeederSystem);
    }

    return loadingFeederSystem;
}


/**
 * Get all the schedules from the save text file and convert them into feeding schedules struct.
 * @return an array of user saved feeding schedules for the feeder system.
 */
feedingSchedule* getScheduleDataFromFile(){
    char lineText[LINE_SIZE];
    char* endPtr;
    // An array of schedules pointer does not contain data about the array size.
    // So this getAmountOfSchedules() will then get the amount of schedules from the save text file.
    int amountOfSchedules = getAmountOfSchedules();

    int dayOfWeek;
    char* timeString;
    int numberOfFeed;

    feedingSchedule* arrayOfSchedules = malloc(amountOfSchedules * sizeof(feedingSchedule));
    // Read mode of the save text file.
    FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");

    // If there is system date time save.
    if (isSystemDateTimeSaveExist() == 1){
        // Skip the first 3 lines of the save text file.
        getNextLineText(lineText,filePtr,3);
    }

    else{
        // Skip the first line of the save text file.
        getNextLineText(lineText,filePtr,1);
    }

    // Read each line of schedules data in the save text file.
    for (int i = 0; i<amountOfSchedules; i++){

        // Get next line of schedule data.
        fgets(lineText,LINE_SIZE,filePtr);

        // Get the day of the week data and convert it into an integer.
        dayOfWeek = strtol((strtok(lineText, " ")),&endPtr,10);

        // Get the time string part of the schedule data.
        timeString = strtok(NULL, " ");

        // Get the number of feed part of the schedule data and convert it to an integer value.
        numberOfFeed = strtol((strtok(NULL, " ")),&endPtr,10);

        // Pass the day of the week value to the feeder system.
        arrayOfSchedules[i].dayOfWeek = dayOfWeek;

        // Convert the time string to a time struct which contains hour, minute and second values.
        arrayOfSchedules[i].time = *timeStringToSystemTime(timeString);

        // Pass the number of feed  value to the feeder system.
        arrayOfSchedules[i].numberOfFeed = numberOfFeed;
    }

    fclose(filePtr);

    return arrayOfSchedules;
}


/**
 * Reads from the save text file and count how many saved schedules are there.
 * @return the number of saved schedules within the save text file.
 */
int getAmountOfSchedules(){
    char lineText[LINE_SIZE];
    int amountOfSchedules = 0;

    // Read mode on the save text file.
    FILE* filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");

    // If there is no File of this name then return 0.
    if (filePtr == NULL){
        return 0;
    }

    // If the system date time save exist within the save text file then skip 3 lines of text.
    if (isSystemDateTimeSaveExist()){
        getNextLineText(lineText,filePtr,3);
    }

    // Skip one lines of text within the save text file.
    else{
        getNextLineText(lineText,filePtr,1);
    }

    // Keep reading each line of save text file that contains schedules data.
    while(fgets(lineText, LINE_SIZE, filePtr) != NULL){
        amountOfSchedules++;
    }

    fclose(filePtr);

    return amountOfSchedules;
}


/**
 * Checks if there is any system date time save within the save text file.
 * @return 1 if there is a system date time save, otherwise 0.
 */
int isSystemDateTimeSaveExist(){
    FILE* filePtr;
    // Read mode on the save text file.
    filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");

    // File cannot be found so the system dat time does not exist.
    if (filePtr == NULL){
        return 0;
    }

    char lineText[LINE_SIZE];

    // Read every line within the save text file.
    while(fgets(lineText, sizeof(lineText), filePtr) != NULL){
        // If the line text file contain system date time save identifier
        if (strcmp(lineText, "systemDateTime:\n") == 0){
            return 1;
        }
    }
    fclose(filePtr);

    // No line within the save text file had the system date time identifier.
    return 0;
}


/**
 * Checks if there is any system schedule save within the save text file.
 * @return 1 if there is a system schedule save, otherwise 0.
 */
int isScheduleTimeSaveExist(){
    FILE* filePtr;
    // Read mode on the save text file.
    filePtr = fopen("../fishFeederFirmwareSaveTextFile.txt", "r");

    // Save text file does not exist therefore there is no schedule saves.
    if (filePtr == NULL){
        return 0;
    }

    char lineText[LINE_SIZE];

    // Get every line within the save text file.
    while(fgets(lineText, sizeof(lineText), filePtr) != NULL){
        // If the line contain schedules save identifier.
        if (strcmp(lineText, "scheduleTime:\n") == 0){
            return 1;
        }
    }

    fclose(filePtr);

    // No schedules saves can be found within the save text file.
    return 0;
}


/**
 * Removes the zero padding of values from the save text file.
 * @param value
 */
void removeZeroPadding(char* value){
    // n will hold the index after all the zeros padding.
    int n;

    //Checks through all the element of date or time string by iterating through to find a char that is not
    //a zero and ensures the zero found is not the null-terminated string literal.
    if((n = strspn(value, "0")) != 0 && (value[n] != '\0')) {

        //Copies the part of the date or time substring that does not contain the 0 padding by
        //Overwriting the original string pointer.
        strcpy(value, &value[n]);
    }
}


/**
 * Load the system clock with the saved system date time values.
 * @param systemDateTime is the saved system date time values from the save text file.
 */
void loadClock(const systemDateTime* systemDateTime) {
    systemDate* date = systemDateTime -> date;
    systemTime* time = systemDateTime -> time;

    // Parameter: SEC MIN HOUR DAY MONTH YEAR
    // Sets the system date time from date time in the save text file.
    clockSet(time -> second, time -> minute, time -> hour, date -> day, date -> month, date -> year);
}


/**
 * Read lines from the save text file.
 * @param lineText where the text will be saved.
 * @param filePtr which start from where the file reader is left off at.
 * @param amountOfRead specify the amount of read to the fish feeder save text file.
 */
void getNextLineText(char* lineText,FILE* filePtr, int amountOfRead){
    for (int i = 0; i < amountOfRead; i++){
        fgets(lineText, LINE_SIZE, filePtr);
    }
}


