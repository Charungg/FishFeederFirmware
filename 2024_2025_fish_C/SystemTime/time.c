#include <stdio.h>
#include <stdlib.h>

#include "fish.h"
#include "time.h"
#include "dateTime.h"
#include "userControl.h"
#include "menu.h"
#include "file.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8

#define LINE_SIZE 80


void runningSetupTime(feedingScheduleData* feedingScheduleData) {
    char* result;
    int running_setup_time = 1;
    int prev_second = clockSecond();
    int AFKTimer = 0;
    int currentOption = 0;

    printf("Short press to navigate, long press to change value\n");
    textData* setupTimeTexts = setupTimeMenuTexts();
    systemDateTime* dateTime = getSystemDateTime();

    displaySetupTime(dateTime,setupTimeTexts ,&currentOption);

    do {
        msleep(500L);
        result = buttonState(); // get the button state from the JavaFX application

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupTime(dateTime,setupTimeTexts, &currentOption);
                currentOption = 0;
            }
        }


        if (isLongPressed(result, &AFKTimer)){
            running_setup_time = selectSetupTimeOption(&currentOption,setupTimeTexts, dateTime);
            free(result);
        }

        // check the result and quit this look if the button is short pressed
        else if (isShortPressed(result, &AFKTimer)){
            currentOption = navigateSetupTime(dateTime,setupTimeTexts, currentOption);
        }

    }while(running_setup_time);
    free(setupTimeTexts);
    free(dateTime);
}

textData* setupTimeMenuTexts(){
    const int n = 2;
    textData* textDataPtr = malloc(sizeof(textData)*n);
    textDataPtr[0] = setupTextData(0,7,"Exit", 1,"red","black");
    textDataPtr[1] = setupTextData(17,7,"Save", 1,"#7CFC00","black");
    return textDataPtr;
}


void displaySetupTime(const systemDateTime* systemDateTime, const textData* setupTimeTexts, const int* currentOption) {
    displayColour("white", "black");
    displayClear();

    displayDateBox();
    displayTimeBox();

    displaySystemDateTime(systemDateTime,currentOption);
    displayOptionText(&setupTimeTexts[0],0);
    displayOptionText(&setupTimeTexts[1],0);
}


void displaySystemDateTime(const systemDateTime* systemDateTime, const int* currentOption){
    displayDay(systemDateTime -> date,*currentOption == 0);
    displayMonth(systemDateTime -> date,*currentOption == 1);
    displayYear(systemDateTime -> date, *currentOption == 2);

    displayHour(systemDateTime -> time,*currentOption == 3);
    displayMinute(systemDateTime -> time,*currentOption == 4);
    displaySecond(systemDateTime -> time,*currentOption == 5);
}


int selectSetupTimeOption(int *currentOption, const textData* setupTimeTexts, systemDateTime* dateTime){
    void (*incrementFunction)(systemDateTime*);

    switch(*currentOption){
        case 0:
            incrementFunction = &incrementDay;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption);
            break;

        case 1:
            incrementFunction = &incrementMonth;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption);
            break;

        case 2:
            incrementFunction = &incrementYear;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption);
            break;

        case 3:
            incrementFunction = &incrementHour;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption);
            break;

        case 4:
            incrementFunction = &incrementMinute;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption);
            break;

        case 5:
            incrementFunction = &incrementSecond;
            runningModifyDateTimeValue(incrementFunction, setupTimeTexts, dateTime, currentOption);
            break;

        case 6:
            return 0;

        case 7:
            displayOptionText(&setupTimeTexts[1],0);
            msleep(250);
            displayOptionText(&setupTimeTexts[1],1);
            setSystemDateTimeClock(dateTime);
            // saveSystemDateTime(dateTime);
            break;

        default:
            exit(1);
    }

    return 1;
}


void runningModifyDateTimeValue(void (*incrementFunction)(systemDateTime*), const textData* setupTimeTexts, systemDateTime* time, int *currentOption){
    int running_changing_date_ime = 1;
    char* result;

    int AFKTimer = 0;
    int prev_second =0;

    displaySelected(1);
    do{
        msleep(500L);
        result = buttonState();

        if(isTimeUpdated(&prev_second)){
            if(isUserAFK(&AFKTimer)) {
                displaySetupTime(time, setupTimeTexts, currentOption);
                *currentOption = 0;
                running_changing_date_ime = 0;
            }
        }

        if (isLongPressed(result, &AFKTimer)){
            displaySelected(0);
            running_changing_date_ime = 0;
        }

        else if (isShortPressed(result, &AFKTimer)){
            incrementFunction(time);
        }

    }while(running_changing_date_ime);
}





int navigateSetupTime(const systemDateTime* systemDateTime, const textData* setupTimeTexts, int currentOption){
    switch(currentOption){
        case 0:
            displayDay(systemDateTime -> date,0);
            displayMonth(systemDateTime -> date,1);
            break;
        case 1:
            displayMonth(systemDateTime -> date,0);
            displayYear(systemDateTime -> date,1);
            break;
        case 2:
            displayYear(systemDateTime -> date,0);
            displayHour(systemDateTime -> time,1);
            break;
        case 3:
            displayHour(systemDateTime -> time,0);
            displayMinute(systemDateTime -> time,1);
            break;
        case 4:
            displayMinute(systemDateTime -> time,0);
            displaySecond(systemDateTime -> time,1);
            break;
        case 5:
            displaySecond(systemDateTime -> time,0);
            displayOptionText(&setupTimeTexts[0],1);
            break;
        case 6:
            displayOptionText(&setupTimeTexts[0],0);
            displayOptionText(&setupTimeTexts[1],1);

            break;
        case 7:
            displayOptionText(&setupTimeTexts[1],0);
            displayDay(systemDateTime -> date,1);
            break;
        default:
            exit(1);
    }


    return (currentOption+1)%8;
}