#include "led_process.h"
#include "../utils/file_util.h"
#include <stdio.h>
#include <string.h>

void setLEDBrightness(LED *led, int brightness)
{
    char fullPath[150];
    snprintf(fullPath, sizeof(fullPath), "%s/brightness", led->path);
    if (brightness == 1) {
        writeToFile(fullPath, "1"); // Turn LED on
    } else if (brightness == 0) {
        writeToFile(fullPath, "0"); // Turn LED off
    } else {
        printf("Invalid value. Use 1 (on) or 0 (off) only!!!.\n");
    }
}

void setLEDTrigger(LED *led, char *trigger) 
{
    char fullPath[150];
    snprintf(fullPath, sizeof(fullPath), "%s/trigger", led->path);
    writeToFile(fullPath, trigger);
    //printf("Trigger for %s set to: %s\n", led->name, trigger);
}