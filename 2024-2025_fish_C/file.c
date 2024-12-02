//
// Created by charl on 02/12/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fish.h"

void saveSystemTime(systemTime time) {
    char* text;

    FILE* filePtr;
    filePtr = fopen("../systemTime.txt","w");

    if (filePtr == NULL) {
        printf("Error opening file\n");
    }

    else {
        text = "systemTime:";
        fprintf(filePtr, "%s\n", text);
        fprintf(filePtr, "%02d/%02d/%04d %2d:%02d:%02d\n", time.day, time.month, time.year, time.hour, time.minute, time.second);
    }

    fclose(filePtr);
}

void loadSystemTime() {
    char lineText[100];

    FILE *filePtr;
    filePtr = fopen("../systemTime.txt", "r");

    fgets(lineText, sizeof(lineText), filePtr);
    printf("LineText: %s", lineText);

    if (filePtr == NULL) {
        printf("Error opening file\n");
    }

    else if (strcmp(lineText, "systemTime:\n") == 0) {
        fgets(lineText, sizeof(lineText), filePtr);
        setSystemDateAndTime(lineText);
    }


    printf("-------------------------\n");
    fclose(filePtr);
}

void setSystemDateAndTime(char* lineText) {
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

    loadSetClock(dateTimeArray);
}

void splitDateTimeString(char* dateTimeString, char* delimiter, int* dateTimeArray, int* currentFreeIndex) {
    char* text;

    // Gets the first string part before the delimiter.
    text = strtok(dateTimeString, delimiter);

    // Gets the all the string parts after the first delimiter.
    do {
        // Removes any zeros padding if their any.
        removeZerosPadding(text);
        printf("Value: %s\n", text);

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
        printf("Value after removing padding: %s\n", &value[n]);

        //Copies the part of the date or time substring that does not contain the 0 padding by
        //Overwriting the original string pointer.
        strcpy(value, &value[n]);
        printf("Pointer dereferenced to new value: %s\n", value);
    }
}

void loadSetClock(int* dateTimeArray) {
    clockSet(dateTimeArray[3], dateTimeArray[4], dateTimeArray[3], dateTimeArray[0], dateTimeArray[1], dateTimeArray[2]);
}