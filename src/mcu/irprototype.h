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
void initialise_data_pointers(void);
void initialise_data_struct(void);

#endif /* IRPROTOTYPE_H_*/