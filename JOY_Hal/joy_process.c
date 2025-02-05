#include "joy_process.h"

int init_i2c_bus(char* bus, int address)
{
    int i2c_file_desc = open(bus, O_RDWR);
    if (i2c_file_desc == -1) {
        printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
        perror("Error is:");
        exit(EXIT_FAILURE);
    }
    if (ioctl(i2c_file_desc, I2C_SLAVE, address) == -1) {
        perror("Unable to set I2C device to slave address.");
        exit(EXIT_FAILURE);
    }
    return i2c_file_desc;
}


void write_i2c_reg16(int i2c_file_desc, uint8_t reg_addr, uint16_t value)
{
    int tx_size = 1 + sizeof(value);
    uint8_t buff[tx_size];
    buff[0] = reg_addr;
    buff[1] = (value & 0xFF);
    buff[2] = (value & 0xFF00) >> 8;
    int bytes_written = write(i2c_file_desc, buff, tx_size);
    if (bytes_written != tx_size) {
        perror("Unable to write i2c register");
        exit(EXIT_FAILURE);
    }
}

uint16_t read_i2c_reg16(int i2c_file_desc, uint8_t reg_addr)
{
    // To read a register, must first write the address
    int bytes_written = write(i2c_file_desc, &reg_addr, sizeof(reg_addr));
    if (bytes_written != sizeof(reg_addr)) {
        perror("Unable to write i2c register.");
        exit(EXIT_FAILURE);
    }
    // Now read the value and return it
    uint16_t value = 0;
    int bytes_read = read(i2c_file_desc, &value, sizeof(value));
    if (bytes_read != sizeof(value)) {
        perror("Unable to read i2c register");
        exit(EXIT_FAILURE);
    }
    return value;
}


int interpretJoystick(uint16_t raw_x) {
    
    // Fix endianness
    uint16_t x = (raw_x >> 8) | (raw_x << 8);
    
    uint16_t center_pos = 13200;  // Joystick's center
    uint16_t threshold = 5000; // Threshold to decide

    // Determine X direction
    if (x < center_pos - threshold) {
        return(1);
    } else if (x > center_pos + threshold) {
        return(0);
    } else {
        return(2);
    }
}
