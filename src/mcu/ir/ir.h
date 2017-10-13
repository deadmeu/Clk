/*
 * ir.h
 *
 * Written by Alex Subaric.
 * Modified by Huy Nguyen.
 */ 

#ifndef IR_H_
#define IR_H_

#include <stdint.h>

#include "unique_types.h"

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define BAUD_IR 300
#define UBRR_IR ((F_CPU/16/BAUD_IR) - 1)

#define BUFFER_FULL     1

/*****************************************************************************
 * Data structures and markers relevant to receiving bytestreams.
 ****************************************************************************/

typedef struct {
    wtype_t weather_one;
    wtype_t weather_two;
    uint8_t time_alarm_hour;
    uint8_t time_alarm_mins;
    uint8_t time_alarm_secs;
    uint8_t time_clock_hour;
    uint8_t time_clock_mins;
    uint8_t time_clock_secs;
} bystream_t;

typedef enum {
    WEATHER_ONE,
    WEATHER_TWO,
    ALARM_HOUR,
    ALARM_MINS,
    ALARM_SECS,
    CLOCK_HOUR,
    CLOCK_MINS,
    CLOCK_SECS
} cbufindex_t;

/*****************************************************************************
 * Functions for handling the serial stream and data buffers.
 ****************************************************************************/

void init_ir(void);
void clear_receive_buffer(void);
void check_receive_buffer(void);
void ir_update_data(void);
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size);

uint8_t clock_is_updating(void);
uint8_t increment_size_marker(void);
uint8_t increment_recv_marker(void);
uint8_t add_byte_to_buffer(uint8_t value);

/*****************************************************************************
 * Functions for extracting data from the buffer and updating the clock's
 * state.
 ****************************************************************************/

void ir_set_data(void);

#endif /* IR_H_ */
