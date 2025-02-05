#include <stdio.h>
#include <stdlib.h>
#include "JOY_Hal/joy_process.h"
#include "LED_Hal/led_process.h"
#include "utils/time_util.h"

// Device bus & address
#define I2CDRV_LINUX_BUS "/dev/i2c-1"
#define I2C_DEVICE_ADDRESS 0x48
// Register in TLA2024
#define REG_CONFIGURATION 0x01
#define REG_DATA 0x00
// Configuration reg contents for continuously sampling different channels
#define TLA2024_CHANNEL_CONF_0 0x83C2

#define TLA2024_CHANNEL_CONF_1 0x83D2

void initialize_LEDs(LED leds[]){
    setLEDBrightness(&leds[0], 0);
    setLEDBrightness(&leds[1], 0);
    setLEDTrigger(&leds[0], "none");
    setLEDTrigger(&leds[1], "none");
}
void turnLED_GreenRed(LED leds[]){
    setLEDBrightness(&leds[0], 1);
    sleepForMs(250);
    setLEDBrightness(&leds[0], 0);
    setLEDBrightness(&leds[1], 1);
    sleepForMs(250);
    setLEDBrightness(&leds[1], 0);
    setLEDBrightness(&leds[0], 1);
    sleepForMs(250);
    setLEDBrightness(&leds[0], 0);
    setLEDBrightness(&leds[1], 1);
    sleepForMs(250);
    setLEDBrightness(&leds[1], 0);
    setLEDBrightness(&leds[0], 1);

}

void flashLight(LED *led){
for (int i = 0; i < 5; i++) {
        setLEDBrightness(led, 1);
        sleepForMs(100);
        setLEDBrightness(led, 0);
        sleepForMs(100);
    }
}

int isRightPress(int i, int i2c_file_desc){
    write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_CONF_0);
    uint16_t raw_y = read_i2c_reg16(i2c_file_desc, REG_DATA);
    write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_CONF_1);
    uint16_t raw_x = read_i2c_reg16(i2c_file_desc, REG_DATA);
    //printf("value %d,   %d  values \n", i, interpretJoystick(raw_y));
    if((interpretJoystick(raw_y) == i)){
        return 1;
    }
    else if((interpretJoystick(raw_y) == 2) && (interpretJoystick(raw_x) == 2)){
        return 3;
    }
    else if((interpretJoystick(raw_y) != i) && (interpretJoystick(raw_x) == 2)){
        return 0;
    }
    else if ((interpretJoystick(raw_x) != 2)){
        return 2;
    }
    
}
bool joy_moved(int i2c_file_desc){
        write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_CONF_0);
        uint16_t raw_y = read_i2c_reg16(i2c_file_desc, REG_DATA);
        write_i2c_reg16(i2c_file_desc, REG_CONFIGURATION, TLA2024_CHANNEL_CONF_1);
        uint16_t raw_x = read_i2c_reg16(i2c_file_desc, REG_DATA);
        if((interpretJoystick(raw_x) < 2) || (interpretJoystick(raw_y) < 2 ) ){
            return true;
        }
        else{
            return false;
        }
}

int main()
{
    printf("Hello embedded world, from Gurkirat Singh!....\n\n");
    printf("When the LEDs light up, press the joystick in that direction\nGreen light means UP and Red light means Down\n(Press left or right to exit) \n");
    int i2c_file_desc = init_i2c_bus(I2CDRV_LINUX_BUS, I2C_DEVICE_ADDRESS);

    LED leds[] = {
        {"Green", LED_PATH "ACT"},
        {"Green", LED_PATH "PWR"},
    };
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    
    long long best_time = 6000; 
    long long reaction_time = 0;
    long long starting_time = 0;
    while(true) {
        printf("Get ready....\n");

        if(joy_moved(i2c_file_desc)){
            printf("Wait for the light to light up, Please leave joystick...\n");
            sleepForMs(500);
            continue;
        }
        initialize_LEDs(leds);
        turnLED_GreenRed(leds);
        initialize_LEDs(leds);
        float randomSleep = 0.5 + ((float)rand() / RAND_MAX) * (3.0 - 0.5);

        if(joy_moved(i2c_file_desc)){
            printf("Wait for the light to light up, Please leave joystick...\n");
            sleepForMs(500);
            continue;
        }

        sleepForMs((int)(randomSleep * 1000));

        if(joy_moved(i2c_file_desc)){
            printf("Too soon..Wait for the light to light up, Please leave joystick...\n");
            sleepForMs(500);
            continue;
        }

        int flag = rand() % 2;

        if(flag == 0){
            printf("Press UP now!\n");

            setLEDBrightness(&leds[0], 1);
            starting_time = getTimeInMs();
            while(true){
                int r = isRightPress(0, i2c_file_desc);
                if(r == 1){
                    reaction_time = getTimeInMs();
                    reaction_time = reaction_time - starting_time;
                    printf("Correct!!\n");
                    initialize_LEDs(leds);
                    flashLight(&leds[0]);
                    if(reaction_time < best_time){
                        best_time = reaction_time;
                        printf("New best time!\nYour reaction time was (%lld) ms; best so far in game is (%lld) ms.\n", best_time, best_time);
                        
                    }
                    break;
                }
                else if(r == 0){
                    printf("Incorrect!!\n");
                    initialize_LEDs(leds);
                    flashLight(&leds[1]);
                    break;
                }
                else if(r == 2){
                    printf("User selected to quit.\n");
                    initialize_LEDs(leds);
                    return 0;
                }
                else if (r == 3){
                    reaction_time = getTimeInMs();
                    if(reaction_time - starting_time > 5000 ){
                        printf("No input within 5000ms; quitting!!!\n");
                        initialize_LEDs(leds);
                        return 0;
                    }
                }
            }
            continue;
        }

        if(flag == 1){
            printf("Press DOWN now!\n");
            setLEDBrightness(&leds[0], 0);
            setLEDBrightness(&leds[1], 1);
            starting_time = getTimeInMs();
            while(true){
                int r = isRightPress(1, i2c_file_desc);
                if(r == 1){
                    reaction_time = getTimeInMs();
                    reaction_time = reaction_time - starting_time;
                    printf("Correct!!\n");
                    initialize_LEDs(leds);
                    flashLight(&leds[0]);
                    if(reaction_time < best_time){
                        best_time = reaction_time;
                        printf("New best time!\nYour reaction time was (%lld) ms; best so far in game is (%lld) ms.\n", best_time, best_time);
                        
                    }
                    break;
                }
                else if(r == 0){
                    printf("Incorrect!!\n");
                    initialize_LEDs(leds);
                    flashLight(&leds[1]);
                    break;
                }
                else if(r == 2){
                    printf("User selected to quit.\n");
                    initialize_LEDs(leds);
                    return 0;
                }
                else if (r == 3){
                    reaction_time = getTimeInMs();
                    if(reaction_time - starting_time > 5000 ){
                        printf("No input within 5000ms; quitting!!!\n");
                        initialize_LEDs(leds);
                        return 0;
                    }
                }
            }
            continue;
            
        }

    }
    // Cleanup I2C access;
    close(i2c_file_desc);
    initialize_LEDs(leds);
    return 0;
}