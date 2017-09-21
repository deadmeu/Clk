/*
 * ir.h
 *
 * Written by Alex Subaric.
 */ 

#ifndef IR_H_
#define IR_H_

#include <stdint.h>

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

void init_data_pointers(void);
void init_data_size_array(void);
void clear_receive_buffer(void);
void check_receive_buffer(void);
void populate_data_struct(void);
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size);
void set_clock_data(void);

#endif /* IR_H_*/
