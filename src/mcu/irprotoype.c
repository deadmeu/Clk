/*
 * irprototype.c
 *
 * Written by Alex Subaric.
 */

 #include <stdint.h>

 #include "irprototype.h"

 #define BITSTREAM_SIZE     1024
 #define BITSTREAMS_SENT    5
 #define STRUCT_ELEMENTS    2

 uint32_t receive_buffer[BITSTREAMS_SENT][BITSTREAM_SIZE];
 uint8_t checked_buffer[BITSTREAM_SIZE];
 uintptr_t *p_data[STRUCT_ELEMENTS];
 
 bitstream data;

 /* Examines each element of the bitstream buffers (stored in the receive buffer), then fills the respective
    element in the checked buffer with the mode value of each element of the bitstream buffers. */
void check_receive_buffer(void) {
    uint8_t ones;
    for (uint16_t i = 0; i < BITSTREAM_SIZE; i++) {
        ones = 0;
        for (uint8_t j = 0; j < BITSTREAMS_SENT; j++) {
            ones += receive_buffer[j][i];
        }
        checked_buffer[i] = (ones > (BITSTREAMS_SENT / 2));
    }
}

/* Assigns pointers from each element in the data struct to each element of the data pointer array. */
void initialise_data_pointers(void) {
    p_data[0] = &data.new_time;
    p_data[1] = &data.alarm_time;
}

/* Copies each data block of the checked buffer into the data struct. */
void initialise_data_struct(void) {
    uint16_t byte_offset = 0;
    for (uint8_t i = 0; i < STRUCT_ELEMENTS; i++) {
        memcpy(p_data[i], checked_buffer + byte_offset, sizeof(p_data[i]));
        byte_offset += sizeof(p_data[i]);
    }
}
