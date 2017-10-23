/*
 * progmem.h
 *
 * Written by Alex Subaric.
 */

#ifndef PROGMEM_H_
#define PROGMEM_H_

#include <stdint.h>
#include "unique_types.h"

#define OFFSET_ADDR			0x00

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
    EEPROM_META_ADDR = 0x0000+OFFSET_ADDR,
    WEATHER_ONE_ADDR = 0x0001+OFFSET_ADDR,
    WEATHER_TWO_ADDR = 0x0002+OFFSET_ADDR,
    ALARM_HOUR_ADDR  = 0x0003+OFFSET_ADDR,
    ALARM_MINS_ADDR  = 0x0004+OFFSET_ADDR,
    ALARM_SECS_ADDR  = 0x0005+OFFSET_ADDR, 
    CLOCK_HOUR_ADDR  = 0x0006+OFFSET_ADDR,
    CLOCK_MINS_ADDR  = 0x0007+OFFSET_ADDR,
    CLOCK_SECS_ADDR  = 0x0008+OFFSET_ADDR
} pmemaddr_t;

typedef enum { EEPROM_DISABLED, EEPROM_ENABLED } eep_t;

void eeprom_update_data(void);
void eeprom_write_data(void);
void eeprom_clear(void);
void eeprom_set_data(void);

uint8_t eeprom_read_data(void);

#endif /* PROGMEM_H_ */
