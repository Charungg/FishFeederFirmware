#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fish.h"
#include "file.h"

#define LINE_SIZE 80

void saveSystemDateTime(systemDateTime* dateTime) {
    FILE* filePtr;
    filePtr = fopen("../systemTime.txt","w");

    if (filePtr == NULL) {
        printf("Error opening file\n");
    }

    else {
        systemDate* date = dateTime -> date;
        systemTime* time = dateTime -> time;
        fprintf(filePtr, "%s\n", "systemTime:");
        fprintf(filePtr, "%02d/%02d/%04d %2d:%02d:%02d\n", date -> day, date -> month, date -> year, time -> hour, time -> minute, time -> second);
    }

    fclose(filePtr);
}

void loadSystemDateTime() {
    char lineText[LINE_SIZE];

    FILE *filePtr;
    filePtr = fopen("../systemTime.txt", "r");

    fgets(lineText, sizeof(lineText), filePtr);
    printf("LineText: %s", lineText);

    if (filePtr == NULL) {
        printf("Error opening file\n");
    }

    else if (strcmp(lineText, "systemTime:\n") == 0) {
        fgets(lineText, sizeof(lineText), filePtr);
        readSystemDateTime(lineText);
    }

    fclose(filePtr);
}

void readSystemDateTime(char* lineText) {
    char* dateString;
    char* timeString;

    int dateTimeArray[6] = {1,2,3,4,5,6};
    int currentFreeIndex = 0;

    // dataString does not contain newline whereas timeString contains newline

    printf("Date And Time: %s",lineText);

    dateString = strtok(lineText, " ");
    printf("dateString: %s\n", dateString);

    timeString = strtok(NULL, " ");
    printf("timeString: %s", timeString);

    splitDateTimeString(dateString, "/", dateTimeArray, &currentFreeIndex);
    splitDateTimeString(timeString, ":", dateTimeArray, &currentFreeIndex);

    loadClock(dateTimeArray);
}

void splitDateTimeString(char* dateTimeString, const char* delimiter, int* dateTimeArray, int* currentFreeIndex) {
    char* text;

    // Gets the first string part before the delimiter.
    text = strtok(dateTimeString, delimiter);

    // Gets the all the string parts after the first delimiter.
    do {
        // Removes any zeros padding if their any.
        removeZerosPadding(text);

        // Adds the specific date or time value to array
        dateTimeArray[*currentFreeIndex] = atoi(text);
        *currentFreeIndex = *currentFreeIndex + 1;

        // Continues to get parts after the delimiter.
        text = strtok(NULL, delimiter);

    }while(text != NULL);
}


void removeZerosPadding(char* value){
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

void loadClock(const int* dateTimeArray) {
    clockSet(dateTimeArray[5], dateTimeArray[4], dateTimeArray[3], dateTimeArray[0], dateTimeArray[1], dateTimeArray[2]);
}