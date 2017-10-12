/*
 * progmem.c
 *
 * Written by Alex Subaric.
 */

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include "progmem.h"
#include "clock.h"

/**************
 * Data element information.
 ***************/
#define N_DATA_ELEMENTS     9
#define N_DATA_BYTES        9
#define EMPTY_VALUE         0xFF

/*****************************************
 * Data variables.
 ******************************************/
pmem_t data;

/**
 * Clock data & eeprom write buffer updating functions
 */

void eeprom_update_data(void) {
    data.eeprom_enabled = eeprom_is_set();
    data.weather_one = get_weather_one();
    data.weather_two = get_weather_two();
    data.time_alarm_hour = get_alarm_hours();
    data.time_alarm_mins = get_alarm_minutes();
    data.time_alarm_secs = get_alarm_seconds();
    data.time_clock_hour = get_clock_hours();
    data.time_clock_mins = get_clock_minutes();
    data.time_clock_secs = get_clock_seconds();
}

void eeprom_set_data(void) {
    // Enable/disable eeprom
    (data.eeprom_enabled == EMPTY_VALUE) ? disable_eeprom() : enable_eeprom();

    // Set the weather data
    if (data.weather_one == EMPTY_VALUE || data.weather_two == EMPTY_VALUE) {
        disable_weather();
    } else {
        enable_weather();
        set_weather(data.weather_one, data.weather_two);
    }

    // Set the alarm time data
    if (data.time_alarm_hour == EMPTY_VALUE 
            || data.time_alarm_mins == EMPTY_VALUE 
            || data.time_alarm_secs == EMPTY_VALUE) {
        disable_alarm();
    } else {
        enable_alarm();
        set_split_alarm_time(data.time_alarm_hour, data.time_alarm_mins, 
            data.time_alarm_secs);
    }

    // Set the clock time data
    if (data.time_clock_hour == EMPTY_VALUE
            || data.time_clock_mins == EMPTY_VALUE
            || data.time_clock_secs == EMPTY_VALUE) {
        // If the clock time was was not read correctly, reset the time to 0.
        set_split_clock_time(MIN_HOUR, MIN_MINUTE, MIN_SECOND);
    } else {
        set_split_clock_time(data.time_clock_hour, data.time_clock_mins, 
                             data.time_clock_secs);
    }
}

/****
 * EEPROM manipulation functions
 *******/

void eeprom_clear(void) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    // Clear all addresses
    for (uint16_t addr = EEPROM_MIN_ADDR; addr < EEPROM_MAX_ADDR; addr++) {
        eeprom_write_byte((uint8_t*) addr, EMPTY_VALUE);
    }

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();
}

void eeprom_write_data(void) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    // Weather data
    eeprom_write_byte((uint8_t*) WEATHER_ONE_ADDR, data.weather_one);
    eeprom_write_byte((uint8_t*) WEATHER_TWO_ADDR, data.weather_two);

    // Alarm time data
    eeprom_write_byte((uint8_t*) ALARM_HOUR_ADDR, data.time_alarm_hour);
    eeprom_write_byte((uint8_t*) ALARM_MINS_ADDR, data.time_alarm_mins);
    eeprom_write_byte((uint8_t*) ALARM_SECS_ADDR, data.time_alarm_secs);

    // Clock time data
    eeprom_write_byte((uint8_t*) CLOCK_HOUR_ADDR, data.time_clock_hour);
    eeprom_write_byte((uint8_t*) CLOCK_MINS_ADDR, data.time_clock_mins);
    eeprom_write_byte((uint8_t*) CLOCK_SECS_ADDR, data.time_clock_secs);

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();
}

uint8_t eeprom_read_data(void) {
    pmem_t temp;                // read buffer struct
    uint8_t ret_val = 1;        // return value - 1 is no error, 0 is error

    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    // Eeprom metadata
    temp.eeprom_enabled = eeprom_read_byte((const uint8_t*) EEPROM_META_ADDR);
    if (temp.eeprom_enabled != EEPROM_DISABLED && temp.eeprom_enabled != EEPROM_ENABLED 
                                 && temp.eeprom_enabled != EMPTY_VALUE) {
        ret_val = 0;
        temp.eeprom_enabled = EMPTY_VALUE;
    }

    // Weather data
    // Weather one
    temp.weather_one = eeprom_read_byte((const uint8_t*) WEATHER_ONE_ADDR);
    if (temp.weather_one > WINDY && temp.weather_one != EMPTY_VALUE) {
        ret_val = 0;
        temp.weather_one = EMPTY_VALUE;
    }

    // Weather two
    temp.weather_two = eeprom_read_byte((const uint8_t*) WEATHER_TWO_ADDR);
    if (temp.weather_two > WINDY && temp.weather_two != EMPTY_VALUE) {
        ret_val = 0;
        temp.weather_two = EMPTY_VALUE;
    }

    // Alarm time data
    // Hour
    temp.time_alarm_hour = eeprom_read_byte((const uint8_t*) ALARM_HOUR_ADDR);
    if (temp.time_alarm_hour > MAX_HOUR && temp.time_alarm_hour != EMPTY_VALUE) {
        ret_val = 0;
    }

    // Minutes
    temp.time_alarm_mins = eeprom_read_byte((const uint8_t*) ALARM_MINS_ADDR);
    if (temp.time_alarm_mins > MAX_MINUTE && temp.time_alarm_mins != EMPTY_VALUE) {
        ret_val = 0;
    }

    // Seconds
    temp.time_alarm_secs = eeprom_read_byte((const uint8_t*) ALARM_SECS_ADDR);
    if (temp.time_alarm_secs > MAX_SECOND && temp.time_alarm_secs != EMPTY_VALUE) {
        ret_val = 0;
    }

    // Clock time data
    // Hour
    temp.time_clock_hour = eeprom_read_byte((const uint8_t*) CLOCK_HOUR_ADDR);
    if (temp.time_clock_hour > MAX_HOUR && temp.time_clock_hour != EMPTY_VALUE) {
        ret_val = 0;
    }

    // Minutes
    temp.time_clock_mins = eeprom_read_byte((const uint8_t*) CLOCK_MINS_ADDR);
    if (temp.time_clock_mins > MAX_MINUTE && temp.time_clock_mins != EMPTY_VALUE) {
        ret_val = 0;
    }

    // Seconds
    temp.time_clock_secs = eeprom_read_byte((const uint8_t*) CLOCK_SECS_ADDR);
    if (temp.time_clock_secs > MAX_SECOND && temp.time_clock_secs != EMPTY_VALUE) {
        ret_val = 0;
    }

    // Copy the contents of the buffer struct to the data struct
    data = temp;

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();

    return ret_val;
}
