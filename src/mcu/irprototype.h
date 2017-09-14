/*
 * irprototype.h
 *
 * Written by Alex Subaric.
 */ 

#ifndef IRPROTOTYPE_H_
#define IRPROTOTYPE_H_

#include <stdint.h>

typedef struct bitstream {
    uint32_t new_time;
    uint32_t alarm_time;
} bitstream;

void check_receive_buffer(void);
void clear_receive_buffer(void);
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size);
void init_data_pointers(void);
void init_data_size_array(void);
void init_data_struct(void);

#endif /* IRPROTOTYPE_H_*/