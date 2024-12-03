
void saveSystemDateTime();
void loadSystemDateTime();
void readSystemDateTime(char* lineText);
void splitDateTimeString(char* timeString, char* delimiter, int* dateTimeArray, int* currentFreeIndex);
void removeZerosPadding(char* value);
void loadClock(int* dateTimeArray);
