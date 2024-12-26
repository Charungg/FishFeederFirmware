#ifndef USERCONTROL_H
#define USERCONTROL_H

// Checks if the button state from the JavaFX GUI has been either short or long pressed.
int isLongPressed(const char* result, int* AFKTimer);
int isShortPressed(const char* result, int* AFKTimer);
#endif // USERCONTROL_H
