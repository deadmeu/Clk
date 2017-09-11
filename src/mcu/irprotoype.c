/*
 * irprototype.c
 *
 * Written by Alex Subaric.
 */

 #include <stdint.h>
 #include <string.h>

 #include "irprototype.h"

 /* Constants */
 #define BYTESTREAM_SIZE    8
 #define BYTESTREAMS_SENT   3
 #define DATA_ELEMENTS      2
 #define CHAR_BIT           8       // taken from <limits.h>

 /* Arrays */
 uint8_t receive_buffer[BYTESTREAMS_SENT][BYTESTREAM_SIZE];
 uint8_t checked_buffer[BYTESTREAM_SIZE];
 uint8_t data_size_array[DATA_ELEMENTS];
 void *p_data[DATA_ELEMENTS];
 
 /* Received bytestream struct */
 bytestream data;
 
 /* Examines each element of the bytestream buffers (stored in the receive buffer), then fills the respective
    element in the checked buffer with the mode value of each element of the bytestream buffers. */
void check_receive_buffer(void) {
    uint8_t ones;
    for (uint16_t byte = 0; byte < BYTESTREAM_SIZE; byte++) {
        checked_buffer[byte] = 0x00;
        for (uint8_t bit = 0; bit < CHAR_BIT; bit++) {
            ones = 0;
            for (uint8_t i = 0; i < BYTESTREAMS_SENT; i++) {
                ones += (receive_buffer[i][byte] >> bit) & 0x01;   // examine right-most bit (0th bit) after shifting
            }
            checked_buffer[byte] |= ((ones > (BYTESTREAMS_SENT / 2)) << bit);
        }
    }
}

/* Assigns pointers from each element in the data struct to each element of the data pointer array. */
void init_data_pointers(void) {
    p_data[0] = &data.new_time;
    p_data[1] = &data.alarm_time;
}

/* Stores the size of the data element in an array with maintained order. */
void init_data_size_array(void) {
    data_size_array[0] = sizeof(data.new_time);
    data_size_array[1] = sizeof(data.alarm_time);
}

/* Copies an unsigned integer from one array to another, and changes the endianness. */
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size) {
    for (uint8_t i = 0; i < size; i ++) {
        dest[i] = src[(size - i) - 1];
    }
}

/* Copies each data block of the checked buffer into the data struct. */
void init_data_struct(void) {
    uint16_t byte_offset = 0;
    uint8_t temp[sizeof(uint32_t)];

    for (uint8_t i = 0; i < DATA_ELEMENTS; i++) {
        convert_endianness(temp, checked_buffer + byte_offset, data_size_array[i]);
        memcpy(p_data[i], temp, data_size_array[i]);
        byte_offset += data_size_array[i];
    }
}

/* Clears the receive buffer to prevent dereferencing of garbage. */
void clear_receive_buffer(void) {
    for (uint8_t i = 0; i < BYTESTREAMS_SENT; i++) {
        for (uint8_t j = 0; j < BYTESTREAM_SIZE; j++) {
            receive_buffer[i][j] = 0;
        }
    }
}

 /* Example usage. */
// int main() {
//     clear_receive_buffer();

//     // First transmission
//     // first 32 bits (new_time)
//     receive_buffer[0][0] = 0b10001111;  // first byte of 2.4 billion
//     receive_buffer[0][1] = 0b00001101;  // second byte of 2.4 billion
//     receive_buffer[0][2] = 0b00011000;  // third byte of 2.4 billion
//     receive_buffer[0][3] = 0b00000000;  // fourth byte of 2.4 billion
    
//     // next 32 bits (alarm_time)
//     receive_buffer[0][4] = 0;
//     receive_buffer[0][5] = 0;
//     receive_buffer[0][6] = 0;
//     receive_buffer[0][7] = 120;

//     // Second transmission
//     // first 32 bits (new_time)
//     receive_buffer[1][0] = 0b10001111;  // first byte of 2.4 billion
//     receive_buffer[1][1] = 0b00001101;  // second byte of 2.4 billion
//     receive_buffer[1][2] = 0b00011000;  // third byte of 2.4 billion
//     receive_buffer[1][3] = 0b00000000;  // fourth byte of 2.4 billion
    
//     // next 32 bits (alarm_time)
//     receive_buffer[1][4] = 0;
//     receive_buffer[1][5] = 0;
//     receive_buffer[1][6] = 0;
//     receive_buffer[1][7] = 120;

//     // Third transmission (corrupted)
//     // first 32 bits (new_time)
//     receive_buffer[2][0] = 0;
//     receive_buffer[2][1] = 0;
//     receive_buffer[2][2] = 0;
//     receive_buffer[2][3] = 55;

//     // next 32 bits (alarm_time)
//     receive_buffer[2][4] = 0;
//     receive_buffer[2][5] = 0;
//     receive_buffer[2][6] = 0;
//     receive_buffer[2][7] = 111;

//     check_receive_buffer();
//     init_data_pointers();
//     init_data_size_array();
//     init_data_struct();

//     printf("\nNew time = %u\n", data.new_time);      // 2400000000
//     printf("\nAlarm time = %u\n", data.alarm_time);  // 120
// }