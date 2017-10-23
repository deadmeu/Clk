/*
 * rtc.c
 *
 * Written by Huy Nguyen.
 * Modified by Alex Subaric.
 * A copy of the i2c library used to implement this functionality can be
 * found at
 * http://homepage.hispeed.ch/peterfleury/doxygen/avr-gcc-libraries/group__\
 * pfleury__ic2master.html
 * This software uses libraries licensed under GNU GPL Version 3.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>

#include "rtc.h"
#include "clock.h"
#include "i2c_lib/i2c_master.h"

#define MCP7940N_WRITE  0b11011110
#define MCP7940N_READ   0b11011111

#define ONES(t)      (t % 10)
#define TENS(t)      ((t / 10) << 4)
#define MASK_ONES(t) (t & ((1 << ONE0_BIT) | (1 << ONE1_BIT) | (1 << ONE2_BIT) \
                                                             | (1 << ONE3_BIT)))
#define MASK_TENS(t) ((t & ((1 << TEN0_BIT) | (1 << TEN1_BIT) \
                                            | (1 << TEN2_BIT))) >> 4)

static void set_hours(uint8_t hours);
static void set_minutes(uint8_t minutes);
static void set_seconds(uint8_t seconds);
static void set_data(uint8_t addr, uint8_t value);

static uint8_t read_hours(void);
static uint8_t read_minutes(void);
static uint8_t read_seconds(void);
static uint8_t read_data(uint8_t addr);

// Initialises the RTC
void init_rtc(void) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    // Setup i2c communication for the RTC
    i2c_init();

    uint8_t retval;     // stores the return value after reading each register

    // Start the RTC
    i2c_start(MCP7940N_WRITE);  // start i2c in write mode
    i2c_write(RTCSEC_ADDR);     // set the pointer to the seconds register
    i2c_stop();

    i2c_start(MCP7940N_READ);   // start i2c in read mode
    retval = i2c_read_ack();
    i2c_stop();

    i2c_start(MCP7940N_WRITE);
    i2c_write(RTCSEC_ADDR);
    // Enable the ST_BIT bit.
    i2c_write(retval | (1 << ST_BIT));
    i2c_stop();

    // Enable the use of the battery and enable the oscillator
    i2c_start(MCP7940N_WRITE);
    i2c_write(RTCWKDAY_ADDR);   // set the pointer to the wkday register.
    i2c_stop();

    i2c_start(MCP7940N_READ);
    retval = i2c_read_ack();
    i2c_stop();

    i2c_start(MCP7940N_WRITE);
    i2c_write(RTCWKDAY_ADDR);
    // Enable the VBATEN and OSCRUN bits.
    i2c_write(retval | (1 << VBATEN_BIT) | (1 << OSCRUN_BIT));
    i2c_stop();

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();
}

// Returns 1 if the rtc has started
uint8_t rtc_started(void) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    i2c_start(MCP7940N_WRITE);
    i2c_write(RTCSEC_ADDR);     // set the pointer to the seconds register.
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t retval = i2c_read_ack();
    i2c_stop();

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();

    // ST_BIT is set when the rtc is started.
    return retval & (1 << ST_BIT);
}

// Returns 1 if the rtc is running
uint8_t rtc_running(void) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    i2c_start(MCP7940N_WRITE);
    i2c_write(RTCWKDAY_ADDR);    // set the pointer to the wkday register.
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t retval = i2c_read_ack();
    i2c_stop();

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();

    // OSCRUN is set if the oscillator is running.
    return retval & (1 << OSCRUN_BIT);
}

// Sets the rtc time using the hour, minute, second input arguments
uint8_t rtc_set_split_time(uint8_t h, uint8_t m, uint8_t s) {
    set_hours(h);
    set_minutes(m);
    set_seconds(s);

    if (read_hours() != h || read_minutes() != m || read_seconds() != s) {
        return 0;
    }

    return 1;
}

// Sets the rtc hour value using the input hour argument
static void set_hours(uint8_t hours) {
    set_data(RTCHOUR_ADDR, hours);
}

// Sets the rtc minute value using the input minute argument
static void set_minutes(uint8_t minutes) {
    set_data(RTCMIN_ADDR, minutes);
}

// Sets the rtc seconds value using the input seconds argument
static void set_seconds(uint8_t seconds) {
    set_data(RTCSEC_ADDR, seconds);
}

// Updates the clock data with the values stored in the internal data struct
static void set_data(uint8_t addr, uint8_t value) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    i2c_start(MCP7940N_WRITE); // start i2c in write mode
    i2c_write(addr); // set the pointer to the specified address

    uint8_t write_val = 0x00;
    switch (addr) {
        case RTCSEC_ADDR : write_val |= (1 << ST_BIT);
        case RTCMIN_ADDR :
        case RTCHOUR_ADDR: write_val |= TENS(value) | ONES(value);
                           break;
    }

    i2c_write(write_val);
    i2c_stop();

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();
}

// Returns the rtc time (in seconds)
uint32_t rtc_get_time(void) {
	return CONV_TIME(read_hours(), read_minutes(), read_seconds());
}

// Returns the rtc hour value
static uint8_t read_hours(void) {
    return read_data(RTCHOUR_ADDR) & ~(1 << TIMEFORMAT_BIT);
}

// Returns the rtc minute value
static uint8_t read_minutes(void) {
    return read_data(RTCMIN_ADDR);
}

// Returns the rtc second value
static uint8_t read_seconds(void) {
    return read_data(RTCSEC_ADDR);
}

// Reads values from a specified address of the rtc buffers
static uint8_t read_data(uint8_t addr) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    i2c_start(MCP7940N_WRITE); // start i2c in write mode
    i2c_write(addr); // set the pointer to the specified address
    i2c_stop();

    i2c_start(MCP7940N_READ); // start i2c in read mode
    uint8_t retval = i2c_read_ack();
    i2c_stop();

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();

    return MASK_TENS(retval)*10 + MASK_ONES(retval);
}
