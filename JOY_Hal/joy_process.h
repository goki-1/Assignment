#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdbool.h>

int init_i2c_bus(char* bus, int address);

void write_i2c_reg16(int i2c_file_desc, uint8_t reg_addr, uint16_t value);

uint16_t read_i2c_reg16(int i2c_file_desc, uint8_t reg_addr);

int interpretJoystick(uint16_t raw_x);
