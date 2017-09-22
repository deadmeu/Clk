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

int main(void)
{
    unsigned char ret;

    /* 
     * Initialize I2C library.
     */
    i2c_init();                             

    /* 
     * Write 0x75 to EEPROM address 5 (Byte Write).
     * Set device address and write mode.
     */
    i2c_start_wait(DEV7940+I2C_WRITE);     

    /* 
     * Write address = 5.
     */
    i2c_write(0x05);                        

    /* 
     * Write value 0x75 to EEPROM.
     */
    i2c_write(0x75);                        

    /* 
     * Set stop conditon = release bus.
     */
    i2c_stop();                             

    /* 
     * Read previously written value back from EEPROM address 5.
     * Set device address and write mode.
     */
    i2c_start_wait(DEV7940+I2C_WRITE);     

    /* 
     * Write address = 5.
     */
    i2c_write(0x05);                        

    /* 
     * Set device address and read mode.
     */
    i2c_rep_start(DEV7940+I2C_READ);       

    /* 
     * Read one byte from EEPROM.
     */
    ret = i2c_readNak();                    

    i2c_stop();
    /* 
     * Never reached.
     */
    return 0;   
}
