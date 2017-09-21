/*
 * ir.c
 *
 * Written by Alex Subaric.
 * Modified by Huy Nguyen.
 */

#include "ir.h" 
#include "clock.h" 

/*****************************************************************************
 * Useful constants.
 ****************************************************************************/

#define BYTESTREAMS_RECV 3
//#define DATA_ELEMENTS 8
#define DATA_ELEMENTS 2
#define CHAR_BIT 8 // taken from <limits.h>
//#define BYTESTREAM_SIZE ((2 * sizeof(uint32_t)) + (6 * sizeof(uint8_t)))
#define BYTESTREAM_SIZE (2 * sizeof(uint32_t))

/*****************************************************************************
 * Data structures and markers relevant to receiving bytestreams.
 ****************************************************************************/

/*
 * Markers to correctly place characters read from the serial stream
 * into the buffers.
 */
volatile static uint8_t sizeMarker;
volatile static uint8_t recvMarker;

/*
 * Buffers.
 */
uint8_t receive_buffer[BYTESTREAMS_RECV][BYTESTREAM_SIZE];
uint8_t checked_buffer[BYTESTREAM_SIZE];
uint8_t data_size_array[DATA_ELEMENTS];
void *p_data[DATA_ELEMENTS];

/* 
 * Received bytestream struct 
 */
bytestream data;

/*****************************************************************************
 * Functions for handling the serial stream and data buffers.
 ****************************************************************************/

/* 
 * Assigns pointers from each element in the data struct to each 
 * element of the data pointer array. 
 */
void init_data_pointers(void) {
    p_data[0] = &data.new_time;
    p_data[1] = &data.alarm_time;
    //p_data[2] = &data.weather_type
    //p_data[3] = &data.weather_play_time
    //p_data[4] = &data.alarm_play_time
    //p_data[5] = &data.ante_meridiem_colour
    //p_data[6] = &data.post_meridiem_colour
    //p_data[7] = &data.hour_marker_colour
    
    sizeMarker = 0;
    recvMarker = 0;
}

/* 
 * Stores the size of the data element in an array with maintained order. 
 */
void init_data_size_array(void) {
    data_size_array[0] = sizeof(data.new_time);
    data_size_array[1] = sizeof(data.alarm_time);
    //data_size_array[2] = sizeof(data.weather_type);
    //data_size_array[3] = sizeof(data.weather_play_time);
    //data_size_array[4] = sizeof(data.alarm_play_time);
    //data_size_array[5] = sizeof(data.ante_meridiem_colour);
    //data_size_array[6] = sizeof(data.post_meridiem_colour);
    //data_size_array[7] = sizeof(data.hour_marker_colour);
}

/* 
 * Clears the receive buffer to prevent dereferencing of garbage. 
 */
void clear_receive_buffer(void) {
    for (uint8_t i = 0; i < BYTESTREAMS_RECV; i++) {
        for (uint8_t j = 0; j < BYTESTREAM_SIZE; j++) {
            receive_buffer[i][j] = 0;
        }
    }
}

/* 
 * Examines each element of the bytestream buffers (stored in the 
 * receive buffer), then fills the respective element in the checked 
 * buffer with the mode value of each element of the bytestream buffers. 
 */
void check_receive_buffer(void) {
    uint8_t ones;
    for (uint16_t byte = 0; byte < BYTESTREAM_SIZE; byte++) {
        checked_buffer[byte] = 0x00;
        for (uint8_t bit = 0; bit < CHAR_BIT; bit++) {
            ones = 0;
            for (uint8_t i = 0; i < BYTESTREAMS_RECV; i++) {
                /* 
                 * Examine right-most bit (0th bit) after shifting.
                 */
                ones += (receive_buffer[i][byte] >> bit) & 0x01;   
            }
            checked_buffer[byte] |= ((ones > (BYTESTREAMS_RECV / 2)) << bit);
        }
    }
}

/* 
 * Copies each data block of the checked buffer into the data struct. 
 */
void populate_data_struct(void) {
    uint16_t byte_offset = 0;
    uint8_t temp[sizeof(uint32_t)];

    for (uint8_t i = 0; i < DATA_ELEMENTS; i++) {
        convert_endianness(temp, checked_buffer + byte_offset, 
                data_size_array[i]);
        memcpy(p_data[i], temp, data_size_array[i]);
        byte_offset += data_size_array[i];
    }
}

/* 
 * Copies an unsigned integer from one array to another, and changes the 
 * endianness. 
 */
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size) {
    for (uint8_t i = 0; i < size; i ++) {
        dest[i] = src[(size - i) - 1];
    }
}

void add_char_to_buffer(uint8_t c) {
    receiveBuffer[recvMarker][sizeMarker] = c;
    increment_size_marker();
}

/*
 * The reading of data will be interrupt based.
 * Everytime an interrupt is triggered, the input character will be placed 
 * into the next available position in the input buffer. 
 * The sizeMarker will increment until BYTESTREAM_SIZE - 1. 
 * When the full bytestream has been received, recvMarker will be incremented 
 * until BYTESTREAMS_RECV - 1.
 */

void increment_size_marker(void) {
    sizeMarker++;
    if (sizeMarker == BYTESTREAM_SIZE) {
    /*
     * Wrap around to the start and update the recv marker to note that
     * we've received a whole bytestream once.
     */
        increment_recv_marker();
        sizeMarker = 0;
    }
}

void increment_recv_marker(void) {
    recvMarker++;
    if (recvMarker == BYTESTREAMS_RECV) {
    /*
     * Wrap around.
     * Maybe we should set a flag to note that we've finished reading
     * all the repeeated bytestreams.
     */
        recvMarker = 0;
    }
}

/*****************************************************************************
 * Functions for extracting data from the buffer and updating the clock's
 * state.
 ****************************************************************************/

void set_clock_data(void) {
    /*
     * Update the clock's time.
     */
    if (!set_time(data.new_time)) {
        // raise some error flag?
    }

    /*
     * Update the clock's alarm time.
     */
    if (!set_alarm_time(data.alarm_time)) {
        // raise some error flag?
    }

    /*
     * Update the clock's weather types
     */

    /*
     * Update the weather play time
     */

    /*
     * Update the alarm play time
     */

    /*
     * Update the Ante Meridiem (AM) colour
     */

    /* 
     * Update the Post Meridiem (PM) colour
     */

    /* 
     * Update the clock's hour marker colour
     */
}

int main(void) {
}
