/*
 * ir.c
 *
 * Written by Alex Subaric and Huy Nguyen.
 */

#include <stdint.h>
#include <avr/interrupt.h>
#include <string.h>

#include "ir.h" 
#include "unique_types.h"
#include "clock.h" 

/*****************************************************************************
 * Useful constants.
 ****************************************************************************/

#define BYTESTREAMS_RECV    5
#define DATA_ELEMENTS       6
#define CHAR_BIT            8          // copied from <limits.h>
#define BYTESTREAM_SIZE     6
#define EMPTY_VALUE      0xFF

/*****************************************************************************
 * Data structures and markers relevant to receiving bytestreams.
 ****************************************************************************/

/*
 * Markers to correctly place characters read from the serial stream
 * into the buffers.
 */
static volatile uint8_t size_marker;
static volatile uint8_t recv_marker;
static volatile uint8_t updating_flag;
static volatile uint8_t connection_flag;

/*
 * Buffers.
 */
static volatile uint8_t receive_buffer[BYTESTREAMS_RECV][BYTESTREAM_SIZE];
static volatile uint8_t checked_buffer[BYTESTREAM_SIZE];

/* 
 * Received bytestream struct 
 */
static volatile bystream_t data;

/*****************************************************************************
 * Functions for handling the serial stream and data buffers.
 ****************************************************************************/

/* 
 * Assigns pointers from each element in the data struct to each 
 * element of the data pointer array. 
 */
void init_ir(void) {
    // initialise UART
    USART_init(UBRR_IR);

    // Flush the USART data register.
    USART_flush();

    // Clear flags & buffers
    reset_updating_flag();
    reset_connection_flag();
    clear_receive_buffer();
    reset_buffer_markers();
}

uint8_t connection_established(void) {
    return connection_flag;
}

uint8_t clock_is_updating(void) {
    return updating_flag;
}

void reset_updating_flag(void) {
    updating_flag = 0;
}

void reset_connection_flag(void) {
    connection_flag = 0;
}

void reset_buffer_markers(void) {
    recv_marker = 0;
    size_marker = 0;
}

/* 
 * Clears the receive buffer to prevent dereferencing of garbage. 
 */
void clear_receive_buffer(void) {
    for (uint8_t i = 0; i < BYTESTREAMS_RECV; i++) {
        for (uint8_t j = 0; j < BYTESTREAM_SIZE; j++) {
            receive_buffer[i][j] = EMPTY_VALUE;
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
            for (uint8_t stream = 0; stream < BYTESTREAMS_RECV; stream++) {
                /* 
                 * Examine right-most bit (least-valued bit) after shifting.
                 */
                ones += (receive_buffer[stream][byte] >> bit) & 0x01;   
            }
            checked_buffer[byte] |= ((ones > (BYTESTREAMS_RECV / 2)) << bit);
        }
    }
}

/* 
 * Copies each data block of the checked buffer into the data struct. 
 */
void ir_update_data(void) {
    // uint16_t byte_offset = 0;
    // uint8_t temp[sizeof(uint32_t)];

    // for (uint8_t i = 0; i < DATA_ELEMENTS; i++) {
    //     convert_endianness(temp, checked_buffer + byte_offset, 
    //             data_size_array[i]);
    //     memcpy(p_data[i], temp, data_size_array[i]);
    //     byte_offset += data_size_array[i];
    // }

    data.weather_one     = checked_buffer[WEATHER_ONE];
    data.weather_two     = checked_buffer[WEATHER_TWO];
    data.time_alarm_hour = checked_buffer[ALARM_HOUR];
    data.time_alarm_mins = checked_buffer[ALARM_MINS];
    data.time_clock_hour = checked_buffer[CLOCK_HOUR];
    data.time_clock_mins = checked_buffer[CLOCK_MINS];
}

/* 
 * Copies an unsigned integer from one array to another, and swaps the 
 * endianness (little -> big, big -> little). 
 */
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size) {
    for (uint8_t i = 0; i < size; i ++) {
        dest[i] = src[(size - i) - 1];
    }
}

uint8_t add_byte_to_buffer(uint8_t value) {
    receive_buffer[recv_marker][size_marker] = value;
    return increment_size_marker();
}

/*
 * The reading of data will be interrupt based.
 * Everytime an interrupt is triggered, the input character will be placed 
 * into the next available position in the input buffer. 
 * The size_marker will increment until BYTESTREAM_SIZE - 1. 
 * When the full bytestream has been received, recv_marker will be incremented 
 * until BYTESTREAMS_RECV - 1.
 */

uint8_t increment_size_marker(void) {
    if (++size_marker == BYTESTREAM_SIZE) {
    /*
     * Wrap around to the start and update the recv marker to note that
     * we've received a whole bytestream once.
     */
        size_marker = 0;   
        if (increment_recv_marker()) {
            return BUFFER_FULL;
        }
    }
    return 0;
}

uint8_t increment_recv_marker(void) {
    if (++recv_marker == BYTESTREAMS_RECV) {
    /*
     * Wrap around.
     */
        recv_marker = 0;
        return 1;
    }
    return 0;
}

/*****************************************************************************
 * Functions for extracting data from the buffer and updating the clock's
 * state.
 ****************************************************************************/

void ir_set_data(void) {
    // Set the weather data
    if (data.weather_one == EMPTY_VALUE || data.weather_two == EMPTY_VALUE) {
        disable_weather();
    } else {
        enable_weather();
        set_weather(data.weather_one, data.weather_two);
    }

    // Set the alarm time data
    if (data.time_alarm_hour == EMPTY_VALUE 
            || data.time_alarm_mins == EMPTY_VALUE) {
        disable_alarm();
    } else {
        enable_alarm();
        set_split_alarm_time(data.time_alarm_hour, data.time_alarm_mins, 
                                MIN_SECOND);
    }

    // Set the clock time data
    if (data.time_clock_hour == EMPTY_VALUE
            || data.time_clock_mins == EMPTY_VALUE) {
        // If the clock time was was not transmitted correctly, 
        // reset the time to 0.
        set_split_clock_time(MIN_HOUR, MIN_MINUTE, MIN_SECOND);
    } else {
        set_split_clock_time(data.time_clock_hour, data.time_clock_mins, 
                                MIN_SECOND);
    }
}



ISR(USART_RX_vect) {
    uint8_t byte = UDR0;
    
    if (usart_enabled()) {
        // // Enable clock updating flag
        updating_flag = 1;

        // if (byte == 99) {
        //     connection_flag = 1;
        //     return;
        // }

        if (add_byte_to_buffer(byte) == BUFFER_FULL) {
            disable_usart();
        }
    }
}

/*****************************************************************************
 * Functions to handle serial communication.
 ****************************************************************************/

void USART_init(uint32_t ubrr) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    /*
     * Set the baud rate.
     */
    UBRR0H = (uint8_t) (ubrr >> 8);
    UBRR0L = (uint8_t) (ubrr);

    /*
     * Enable RX and interrupt on receive complete.
     */
    UCSR0B = (1 << RXEN0) | (1 << RXCIE0);

    /*
     * Set frame format: 8 data, 2 stop bits.
     */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();
}

uint8_t USART_getc(void) {
    while (!(UCSR0A & (1 << RXC0))) {
    /*
     * Wait for data to be received.
     */
    }
    return UDR0;
}

void USART_putc(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))) {
    /*
     * Wait for the data register to empty.
     */
    }
    UDR0 = data;
}

void USART_puts(char *data) {
    while (!(UCSR0A & (1 << UDRE0))) {
    /*
     * Wait for the data register to empty.
     */
    }
    for (int i = 0; i < strlen(data); i++) {
        USART_putc(data[i]);
    }
    USART_putc('\r');
    USART_putc('\n');
}

void USART_flush(void) {
    char dummy;
    while (UCSR0A & ( 1 << RXC0)) {
        dummy = UDR0;
        dummy++;
    }
}

/*****************************************************************************
 * Functions to handle the message buffer.
 * The buffer is a matrix / 2d array of characters.
 ****************************************************************************/

void print_buffer(void) {
    cursor_to_top_left();
    for (int i = 0; i < BYTESTREAMS_RECV; i++) {
        for (int j = 0; j < BYTESTREAM_SIZE; j++) {
            USART_putc(receive_buffer[i][j]);
        }
        USART_putc('\r');
        USART_putc('\n');
    }

    /*
     * Output newline to separate multiple printings of the buffer.
     */
    USART_putc('\r');
    USART_putc('\n');
}

/*****************************************************************************
 * Functions to handle terminal io.
 ****************************************************************************/

void initialise_terminal(void) {
    /*
     * Clear the screen.
     */
    USART_puts("\033[2J");

    /*
     * Hide the cursor.
     */
    USART_puts("\033[?25l");
}

void cursor_to_top_left(void) {
    USART_puts("\033[H");
}
