#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "i2c_lib/i2c_master.h"
#include "serialio/serialio.h"

#define MCP7940N_WRITE  0b11011110
#define MCP7940N_READ   0b11011111

void test_osc_1(void);
void test_osc_2(void);
void setup_rtc(void);

void set_seconds(uint8_t seconds);
int read_seconds(void);
void test_seconds(void);

void set_minutes(uint8_t minutes);
int read_minutes(void);
void test_minutes(void);

void set_hours(uint8_t hours);
int read_hours(void);
void test_hours(void);

void test_time(void);

int main() {
    i2c_init();
    USART_init(UBRR);
    setup_rtc();
    test_osc_1();
    test_osc_2();
    test_time();
    return 0;
}

void test_osc_1(void) {
    i2c_start(MCP7940N_WRITE);
    i2c_write(0x00);    // Set the pointer to the seconds register.
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t ret = i2c_read_ack();
    i2c_stop();
    ret &= 0b10000000;
    if (ret == 0) {
        while(1) {
            USART_puts("Oscillator has not started.");
        }
    }
}

void test_osc_2(void) {
    i2c_start(MCP7940N_WRITE);
    i2c_write(0x03);    // Set the pointer to the WKDAY register.
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t ret = i2c_read_ack();
    i2c_stop();
    ret &= 0b00100000;
    if (ret == 0) {
        while(1) {
            USART_puts("Oscillator has stopped.");
        }
    }
}

void test_seconds(void) {
    while (1) {
        int seconds = read_seconds();
        char output[255];
        sprintf(output, "%d", seconds);
        USART_puts(output);
    }
}

void test_minutes(void) {
    while (1) {
        int minutes = read_minutes();
        char output[255];
        sprintf(output, "%d", minutes);
        USART_puts(output);
    }
}

void test_hours(void) {
    while (1) {
        int hours = read_hours();
        char output[255];
        sprintf(output, "%d", hours);
        USART_puts(output);
    }
}

void test_time(void) {
    while (1) {
        int seconds = read_seconds();
        int minutes = read_minutes();
        int hours = read_hours();
        char output[255];
        sprintf(output, "Sec: %d Min: %d Hrs: %d", seconds, minutes, 
                hours);
        USART_puts(output);
    }
}

void setup_rtc(void) {
    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the seconds register.
     */
    i2c_write(0x00);
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t seconds = i2c_read_ack();
    i2c_stop();

    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the seconds register.
     */
    i2c_write(0x00);

    seconds |= 0x80;    // Enable the oscillator.  
    i2c_write(seconds);    
    i2c_stop();

    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the wkday register.
     */
    i2c_write(0x03);
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t wkday = i2c_read_ack();
    i2c_stop();

    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the wkday register.
     */
    i2c_write(0x03);

    wkday |= 0x08;          // Enable the battery.  
    wkday |= 0b00100000;    // Don't disable the osc running flag.
    i2c_write(wkday);    
    i2c_stop();
}

void set_seconds(uint8_t seconds) {
// The argument to this function should be well formed.

    uint8_t secTens = (seconds / 10) << 4;
    uint8_t secOnes = (seconds % 10);

    seconds = 0x80;    // Don't overwrite the oscillator.
    seconds |= secOnes | secTens;

    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the seconds register.
     */
    i2c_write(0x00);
    i2c_write(seconds);    
    i2c_stop();
}

int read_seconds(void) {
    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the seconds register.
     */
    i2c_write(0x00);
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t seconds = i2c_read_ack();
    i2c_stop();

    int secOnes = seconds & 0b00001111;
    int secTens = (seconds & 0b01110000) >> 4;

    return secOnes + secTens * 10;
}

void set_minutes(uint8_t minutes) {
// The argument to this function should be well formed.

    uint8_t minTens = (minutes / 10) << 4;
    uint8_t minOnes = (minutes % 10);

    minutes = minTens | minOnes;

    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the minutes register.
     */
    i2c_write(0x01);
    i2c_write(minutes);    
    i2c_stop();
}

int read_minutes(void) {
    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the minutes register.
     */
    i2c_write(0x01);
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t minutes = i2c_read_ack();
    i2c_stop();

    int minOnes = minutes & 0b00001111;
    int minTens = (minutes & 0b01110000) >> 4;

    return minOnes + minTens * 10;
}

void set_hours(uint8_t hours) {
// The argument to this function should be well formed.

    uint8_t hoursTens = (hours / 10) << 4;
    uint8_t hoursOnes = (hours % 10);

    hours = hoursTens | hoursOnes;

    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the hours register.
     */
    i2c_write(0x02);
    i2c_write(hours);    
    i2c_stop();
}

int read_hours(void) {
    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the hours register.
     */
    i2c_write(0x02);
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t hours = i2c_read_ack();
    i2c_stop();

    int hoursOnes = hours & 0b00001111;
    int hoursTens = (hours & 0b01110000) >> 4;

    return hoursOnes + hoursTens * 10;
}
