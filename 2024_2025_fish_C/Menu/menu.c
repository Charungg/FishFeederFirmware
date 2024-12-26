#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fish.h"
#include "menu.h"
#include "feeder.h"
#include "file.h"
#include "userControl.h"
#include "textStruct.h"
#include "time.h"
#include "scheduleTime.h"

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LINE_SIZE 80


/**
 * Displays and runs the start menu on the GUI
 * It wil display the feeder system next feed time, total number of feed and operating mode.
 * It's what the user will see upon first launching up the feeder firmware.
 * @param title is the name of the program.
 */
void runningStartMenu(char* title){
    int running_start_menu = 1; // continue flag
    int AFKTimer = 0;
    int prev_second = clockSecond();

    // Set up the start menu text for example the next feed time, total number of feed,
    // operating mode and title.
    textData* startMenuTexts = setupStartMenuTexts(title);

    // Loads the feeder system by getting all the schedules from the save text file if there is any.
    // If there is no saved schedules then a default feeder system will be given to the user.
    feederSystem* feeder = loadFeederSystem();

    // Displays all the start menu text and clock time to the GUI.
    displayStartMenu(startMenuTexts, feeder);
    printf("Short press to enter main menu, Long press the button to quit\n");

    do{
        // Check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        // Checks if a second has past.
        if(isTimeUpdated(&prev_second)){
            // Blanks the display after 1 minute of the button no being pressed
            // And also checks if its feeding time from the feeder system.
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
                // Redisplay the start menu if the display has been blanked
                // or when the feeder system has changed values.
                displayStartMenu(startMenuTexts, feeder);
            }
            // Display the updated clock.
            displayClockTime();
        }

        // Checks if the user has long pressed the button.
        if (isLongPressed(result, &AFKTimer)) {
            // Exit out of the start menu program.
            running_start_menu = 0;
            free(result);
        }

        // Checks if the user has short pressed the button.
        if (isShortPressed(result, &AFKTimer)){
            // Displays and runs the main menu.
            runningMainMenu(feeder);
            displayStartMenu(startMenuTexts, feeder);
        }
    }while(running_start_menu);

    free(startMenuTexts);
}


/**
 * Set up the start menu texts to be read for display at all time.
 * @param title is the name of the program.
 * @return start menu texts data.
 */
textData* setupStartMenuTexts(char* title){
    const int n = 4;
    textData* textDataPtr = malloc(sizeof(textData)*n);

    // Setup all 4 start menu texts.
    textDataPtr[0] = setupTextData(0,0,title,2,"#8A2BE2","black");
    textDataPtr[1] = setupTextData(0, 2, "Next Feed Time:", 1, "white", "black");
    textDataPtr[2] = setupTextData(0, 3, "Number Of Feeds:", 1, "white", "black");
    textDataPtr[3] = setupTextData(0, 4, "Operating Mode:", 1, "white", "black");

    return textDataPtr;
}


/**
 * Displays all the start menu texts on the GUI
 * @param startMenuTexts contains start menu texts data.
 * @param feeder contains feeding system data.
 */
void displayStartMenu(textData* startMenuTexts, const feederSystem* feeder){
    displayColour("white", "black");
    displayClear();

    // Display all feeder system data such as next feed time,
    // total number of feeds and operating mode.
    displayNextFeedTime(startMenuTexts, feeder);
    displayTotalNumberOfFeeds(startMenuTexts, feeder);
    displayOperatingMode(startMenuTexts, feeder);


    // Display all start menu texts.
    for (int i=0; i<4; i++){
        displayOptionText(&startMenuTexts[i],0);
    }
}


/**
 * Display system clock time on the GUI
 */
void displayClockTime(){
    // display graphic lines around time
    displayColour("white", "black");
    displayLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH, SCREEN_HEIGHT - 1);
    displayLine(0, SCREEN_HEIGHT - CHAR_HEIGHT*2-1, SCREEN_WIDTH, SCREEN_HEIGHT - CHAR_HEIGHT*2-1);

    // display the time value
    char time[LINE_SIZE];
    snprintf(time, LINE_SIZE, "%02i-%02i-%02i", clockHour(), clockMinute(), clockSecond());
    displayText(SCREEN_WIDTH/2-4*CHAR_WIDTH, SCREEN_HEIGHT-CHAR_HEIGHT*1.5, time, 1);
}


/**
 * Checks if the system time clock has incremented.
 * @param prev_second is the previous second value from the clock before the increment.
 * @return 1 if one second has past, otherwise 0.
 */
int isTimeUpdated(int* prev_second) {
    // Get the current clock second.
    int second = clockSecond();

    // If the previous clock second is not the same as the current clock second.
    // Then a second has past.
    if (second != *prev_second) {
        *prev_second = second;
        return 1;
    }

    // Returns 0 since a second hasn't past.
    return 0;
}


/**
 * Checks if the user has been AFK (away from keyboard) for the past 60 second.
 * @param AFKTimer holds the amount of second the user has been AFK for.
 * @param feeder is the feeder system which will be passed to blank display if the user has been AFK for 60 second.
 * @return 1 if the display has been blanked, otherwise 0.
 */
int isUserAFK(int *AFKTimer, feederSystem* feeder){
    *AFKTimer += 1;

    // If the user has been AFK for 60 second or more.
    if (*AFKTimer >= 60) {
        // Blank the display
        blankDisplay(feeder);

        // Reset the AFK timer back to zero.
        *AFKTimer = 0;
        return 1;
    }

    // Display has not been blanked.
    return 0;
}


/**
 * Once the user is AFK (Away from keyboard) for more than 60 second then it will blank the display.
 * Since the feeder firmware contains an OLEDs screen and to prevent burn in, the screen will be blanked till
 * the user presses the button on the machine.
 * @param feeder is the feeder system.
 */
void blankDisplay(feederSystem* feeder){
    char* result;
    int prev_second = clockSecond();
    int running_blank_display = 1;

    displayColour("white", "grey");
    displayClear();

    do{
        msleep(500L);
        result = buttonState();

        // Check if a second has past.
        if (isTimeUpdated(&prev_second)){
            // Even though the display is blanked, we still need to ensure the fishes are fed.
            // When the feeding schedules is the same as the system time.
            checkFeederSchedule(feeder);
        }

        // If the user press the button in any way, it will exit them out of the blank display and return to their menus.
        if (strcmp(result, "LONG_PRESS") == 0 || strcmp(result, "SHORT_PRESS") == 0){
            running_blank_display = 0;
        }

    }while(running_blank_display);
}


/**
 * Display and run the main menu.
 * Allows the user to navigate either to the feeder menu, setup time menu or setup schedule time menu.
 * @param feeder is the feeder system which has to be passed to every menu to ensure feeding schedules are always checked.
 */
void runningMainMenu(feederSystem* feeder){
    int running_main_menu= 1;
    int AFKTimer = 0;
    int prev_second = clockSecond();
    int currentOption = 0;

    // Set up the main menu texts data for the main menu.
    textData* mainMenuTexts = setupMainMenuTexts();

    // Display the main menu.
    displayMainMenu(mainMenuTexts, currentOption);

    printf("Short press the button to navigate, long to confirm\n");

    do{
        // check for the button state every half second
        msleep(500L);
        char *result = buttonState(); // get the button state from the JavaFX application

        // Checks if a second has past.
        if(isTimeUpdated(&prev_second)){
            // Blanks the display after 1 minute of the button no being pressed
            // And also checks if its feeding time from the feeder system.
            if(isUserAFK(&AFKTimer,feeder) || checkFeederSchedule(feeder)) {
                displayMainMenu(mainMenuTexts, currentOption);
            }
        }

        // Checks if button has been long pressed.
        if (isLongPressed(result, &AFKTimer)) {
            // Run main menu operations depending on what they are currently selecting.
            running_main_menu = runMainMenuOption(feeder, currentOption);
            displayMainMenu(mainMenuTexts, currentOption);
            free(result);
        }

        // Checks if button has been short pressed.
        else if (isShortPressed(result, &AFKTimer)) {
            // Navigate in between the text on the GUI.
            navigateMainMenuTexts(mainMenuTexts, &currentOption);
        }

    }while(running_main_menu);

    free(mainMenuTexts);
}


/**
 * Set up the main menu texts to be used to display main menu texts.
 * @return an array of main menu texts.
 */
textData* setupMainMenuTexts(){
    int n = 4;
    textData* textDataPtr = malloc(sizeof(textData)*n);
    textDataPtr[0] = setupTextData(0,0,"Feeder Menu", 1,"white","black");
    textDataPtr[1] = setupTextData(0,2,"Setup Time", 1,"white","black");
    textDataPtr[2] = setupTextData(0,4,"Setup Schedule Time", 1,"white","black");
    textDataPtr[3] = setupTextData(0,7,"Exit", 1,"red","black");
    return textDataPtr;
}


/**
 * Each text on GUI can be created through the setupTextData() which allows texts to be pre-determined and modifiable.
 * Allows each text to be created easily and ready to be used on the GUI at all time.
 * @param xAxis specify where the text will be placed on the GUI x-axis.
 * @param yAxis specify where the text will be placed on the GUI y-axis.
 * @param text specify the string text on the GUI
 * @param size specify the size of the text.
 * @param foregroundColour specify the foreground colour of the text.
 * @param backgroundColor specify the background colour of the text.
 * @return a text data which contains placement, text, size and colour data.
 */
textData setupTextData(int xAxis, int yAxis, char* text, int size, char* foregroundColour, char* backgroundColor){
    textData newTextData;
    newTextData.xAxis = xAxis;
    newTextData.yAxis = yAxis;
    newTextData.text = text;
    newTextData.size = size;

    newTextData.foregroundColour = foregroundColour;
    newTextData.backgroundColour = backgroundColor;

    return newTextData;
}


/**
 * Displays all main menu texts on the GUI.
 * Whatever text the user is currently selected on, it will display the text differently from the rest.
 * To indicate which option the user is currently on.
 * @param mainMenuTexts is an array of all main menu texts.
 * @param currentOption specify which mein menu texts the user is currently selecting.
 */
void displayMainMenu(const textData* mainMenuTexts, const int currentOption){
    displayColour("white", "black");
    displayClear();

    // Display all main menu texts.
    for (int i=0; i<4; i++){
        // If the main menu text is currently selected by the user then display it as a different colour.
        displayOptionText(&mainMenuTexts[i],currentOption==i);
    }
}


/**
 * Run the main option operations depending on what the user is currently selecting.
 * If the user long press while selecting feeder menu then it will run the feeder menu.
 * Or other menus such as setup time and setup schedules.
 * @param feeder is the feeder system which contains feeding schedules.
 * @param currentOption specify what the user is currently selecting on.
 * @return 1 if the user chose to run other menus, otherwise 0 if the user wants to exist the main menu.
 */
int runMainMenuOption(feederSystem* feeder, int currentOption){
    switch (currentOption) {
        // Run the main menu selected operations.
        case 0:
            runningFeederMenu(feeder);
            break;

        case 1:
            runningSetupTimeMenu(feeder);
            break;

        case 2:
            runningSetupScheduleTimeMenu(feeder);
            break;

        case 3:
            // User chose to exit the main menu.
            return 0;

        // If the user is not currently selecting any of the main menu options.
        // Exit the program as an error has occurred.
        default:
            exit(1);
    }

    // To continue running the main menu once the user exit from other menus.
    return 1;
}


/**
 * Allows the user to navigate in between the main menu options.
 * @param mainMenuOptions contains all the main menu texts.
 * @param currentOption specify what the user is currently selecting on the main menu.
 */
void navigateMainMenuTexts(const textData* mainMenuOptions, int* currentOption){
    // Keep track what the next option the user is going to navigate to.
    int newCurrentOption = ((*currentOption) + 1) % 4;

    // De-select the current user options
    displayOptionText(&mainMenuOptions[*currentOption],0);
    // Select the next user options.
    displayOptionText(&mainMenuOptions[newCurrentOption],1);

    // User now moved to the next main menu option.
    *currentOption = newCurrentOption;
}


/**
 * Display options texts on the GUI.
 * Colour of the option depending on if the user is currently selecting it.
 * @param textData is the text option for the GUI.
 * @param isSelected specify if the user is currently selecting this text option.
 */
void displayOptionText(const textData* textData, int isSelected){
    isTextSelected(textData, isSelected);
    displayText(CHAR_WIDTH*textData -> xAxis, CHAR_HEIGHT*textData -> yAxis, textData -> text, textData -> size);
}


/**
 * Set the colour of the next text that will be displayed on the GUI.
 * Each text data have their own foreground and background colour when they are not selected.
 * @param textData contains the colour of the text.
 * @param isSelected specify whether the user is currently selecting the text.
 */
void isTextSelected(const textData* textData, int isSelected){
    // If the text is selected.
    if (isSelected) {
        // Set the colour to selected colour "black" and gray".
        displayColour("black", "white");
    }

    else{
        // Set the colour to the text data unique colours.
        displayColour(textData -> foregroundColour, textData -> backgroundColour);
    }
}




