/*
 * progmem.h
 *
 * Written by Alex Subaric.
 */

#ifndef PROGMEM_H_
#define PROGMEM_H_

#include <stdint.h>
#include "unique_types.h"

typedef struct {
    uint8_t eeprom_enabled;
    wtype_t weather_one;
    wtype_t weather_two;
    uint8_t time_alarm_hour;
    uint8_t time_alarm_mins;
    uint8_t time_alarm_secs;
    uint8_t time_clock_hour;
    uint8_t time_clock_mins;
    uint8_t time_clock_secs;
} pmem_t;

/*************************************
 * Clock data addresses.
 ************************/
typedef enum {
    EEPROM_MIN_ADDR  = 0x0000,
    EEPROM_MAX_ADDR  = 0x0400,   // the atmega328p has 1024 bytes of eeprom
    EEPROM_META_ADDR = 0x0000,
    WEATHER_ONE_ADDR = 0x0001,
    WEATHER_TWO_ADDR = 0x0002,
    ALARM_HOUR_ADDR  = 0x0003,
    ALARM_MINS_ADDR  = 0x0004,
    ALARM_SECS_ADDR  = 0x0005, 
    CLOCK_HOUR_ADDR  = 0x0006,
    CLOCK_MINS_ADDR  = 0x0007,
    CLOCK_SECS_ADDR  = 0x0008
} pmemaddr_t;

typedef enum { EEPROM_DISABLED, EEPROM_ENABLED } eep_t;

void eeprom_update_data(void);
void eeprom_write_data(void);
void eeprom_clear(void);
void eeprom_set_data(void);

uint8_t eeprom_read_data(void);

#endif /* PROGMEM_H_ */
