#define LED_PATH "/sys/class/leds/"

typedef struct {
    char name[50];  // LED name
    char path[100]; // path to the LED directory
} LED;

void setLEDBrightness(LED *led, int brightness);
void setLEDTrigger(LED *led, char *trigger);
