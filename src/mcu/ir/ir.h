/*
 * ir.h
 *
 * Written by Alex Subaric.
 * Modified by Huy Nguyen.
 */ 

#ifndef IR_H_
#define IR_H_

#include <stdint.h>
#include <string.h>

/*****************************************************************************
 * Data structures and markers relevant to receiving bytestreams.
 ****************************************************************************/

typedef struct bytestream {
    uint32_t new_time;
    uint32_t alarm_time;
    uint8_t weather_type;
    uint8_t weather_play_time;
    uint8_t alarm_play_time;
    uint8_t ante_meridiem_colour;
    uint8_t post_meridiem_colour;
    uint8_t hour_marker_colour;
} bytestream;

/*****************************************************************************
 * Functions for handling the serial stream and data buffers.
 ****************************************************************************/

void init_data_pointers(void);
void init_data_size_array(void);
void clear_receive_buffer(void);
void check_receive_buffer(void);
void populate_data_struct(void);
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size);

void add_char_to_buffer(uint8_t c);
void increment_size_marker(void);
void increment_recv_marker(void);

/*****************************************************************************
 * Functions for extracting data from the buffer and updating the clock's
 * state.
 ****************************************************************************/

void set_clock_data(void);

#endif /* IR_H_ */
