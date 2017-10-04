/* Name: main.c
 * Author: Huy Nguyen, adapted from work by Peter Fleury.
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

/*
 * The i2c line is disabled when on backup power, thus it will not be able
 * to be reprogrammed when on battery backup.
 */

#include "rtc.h"
#include "i2cmaster.h"

/*
 * Slave address for MCP7940N real time clock device.
 * Format           0b1101111x, x=0 for write, x=1 for read.
 *                  The value of x gets added to the address for the desired
 *                  command
 */
#define DEV7940     0b11011110

/*****************************************************************************
 * Example usage of the i2c bus.
 ****************************************************************************/
/*
    unsigned char ret;

     * 
     * Initialize I2C library.
     * 
    i2c_init();                             

     * 
     * Write 0x75 to EEPROM address 5 (Byte Write).
     * Set device address and write mode.
     * 
    i2c_start_wait(DEV7940+I2C_WRITE);     

     * 
     * Write address = 5.
     * 
    i2c_write(0x05);                        

     * 
     * Write value 0x75 to EEPROM.
     * 
    i2c_write(0x75);                        

     * 
     * Set stop conditon = release bus.
     * 
    i2c_stop();                             

     * 
     * Read previously written value back from EEPROM address 5.
     * Set device address and write mode.
     * 
    i2c_start_wait(DEV7940+I2C_WRITE);     

     * 
     * Write address = 5.
     * 
    i2c_write(0x05);                        

     * 
     * Set device address and read mode.
     * 
    i2c_rep_start(DEV7940+I2C_READ);       

     * 
     * Read one byte from EEPROM.
     * 
    ret = i2c_readNak();                    

    i2c_stop();
*/

int main(void)
{
    i2c_init();
    /* 
     * Never reached.
     */
    return 0;   
}

void set_rtc_seconds(int seconds) {
    int tens = seconds / 10;
    int ones = seconds % 10;

    /*
     * Set the RTC address to write mode.
     */
    i2c_start_wait(DEV7940 + I2C_WRITE);

    /*
     * Write the tens value of seconds into the tens registers.
     */
    i2c_write(RTCSEC);
    i2c_write(tens << SECTEN);

    /*
     * Write the ones value of seconds into the ones registers.
     */
    i2c_write(RTCSEC);
    i2c_write(ones << SECONE);

    /*
     * Release the i2c bus.
     */
    i2c_stop();
}

void set_rtc_minutes(int minutes) {
    int tens = minutes / 10;
    int ones = minutes % 10;

    /*
     * Set the RTC address to write mode.
     */
    i2c_start_wait(DEV7940 + I2C_WRITE);

    /*
     * Write the tens value of minutes into the tens registers.
     */
    i2c_write(RTCMIN);
    i2c_write(tens << MINTEN);

    /*
     * Write the ones value of minutes into the ones registers.
     */
    i2c_write(RTCMIN);
    i2c_write(ones << MINONE);

    /*
     * Release the i2c bus.
     */
    i2c_stop();
}

void set_rtc_hours(int hours) {
    int tens = hours / 10;
    int ones = hours % 10;

    /*
     * Set the RTC address to write mode.
     */
    i2c_start_wait(DEV7940 + I2C_WRITE);

    /*
     * Write the tens value of hours into the tens registers.
     */
    i2c_write(RTCHOUR);
    i2c_write(tens << HRTEN);

    /*
     * Write the ones value of hours into the ones registers.
     */
    i2c_write(RTCHOUR);
    i2c_write(ones << HRONE);

    /*
     * Release the i2c bus.
     */
    i2c_stop();
}

int read_rtc_seconds(void) {
    int tens;
    int ones;

    /*
     * Set the RTC address to write mode.
     */
    i2c_start_wait(DEV7940 + I2C_WRITE);
    /*
     * Set the seconds registers as the read target.
     */
    i2c_write(RTCSEC);

    i2c_rep_start(DEV7940 + I2C_READ);
    uint8_t ret = i2c_readNak();

    i2c_stop();
}

int read_rtc_minutes(void) {
    int tens;
    int ones;

    /*
     * Set the RTC address to write mode.
     */
    i2c_start_wait(DEV7940 + I2C_WRITE);
    i2c_write(RTCMIN);

    i2c_rep_start(DEV7940 + I2C_READ);
    uint8_t ret = i2c_readNak();

    i2c_stop();
}

int read_rtc_hours(void) {
    int tens;
    int ones;

    /*
     * Set the RTC address to write mode.
     */
    i2c_start_wait(DEV7940 + I2C_WRITE);
    i2c_write(RTCHOUR);

    i2c_rep_start(DEV7940 + I2C_READ);
    uint8_t ret = i2c_readNak();

    i2c_stop();
}
