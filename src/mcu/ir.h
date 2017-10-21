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
    uint8_t time_clock_hour;
    uint8_t time_clock_mins;
} bystream_t;

typedef enum {
    WEATHER_ONE,
    WEATHER_TWO,
    ALARM_HOUR,
    ALARM_MINS,
    CLOCK_HOUR,
    CLOCK_MINS
} cbufindex_t;

/*****************************************************************************
 * Functions for handling the serial stream and data buffers.
 ****************************************************************************/

void init_ir(void);
void reset_buffer_markers(void);
void clear_receive_buffer(void);
void check_receive_buffer(void);
void ir_update_data(void);
void convert_endianness(uint8_t *dest, uint8_t *src, uint8_t size);
void reset_updating_flag(void);
void reset_connection_flag(void);

uint8_t connection_established(void);
uint8_t clock_is_updating(void);
uint8_t increment_size_marker(void);
uint8_t increment_recv_marker(void);
uint8_t add_byte_to_buffer(uint8_t value);

/*****************************************************************************
 * Functions for extracting data from the buffer and updating the clock's
 * state.
 ****************************************************************************/

void ir_set_data(void);

/*****************************************************************************
 * Functions to handle serial communication.
 ****************************************************************************/

/*
 * This function is called as 
 * USART_init(UBRR);
 */
void USART_init(uint32_t ubrr);

/*
 * Return the next character in the serial buffer.
 */
uint8_t USART_getc(void);

/*
 * Send a character to the serial stream.
 */
void USART_putc(uint8_t data);

/*
 * Send a string to the serial stream.
 */
void USART_puts(char *string);

/*
 * Clear the USART data register.
 */
void USART_flush(void);

/*****************************************************************************
 * Functions to handle the message buffer.
 * The buffer is a matrix / 2d array of characters.
 ****************************************************************************/

/*
 * Print what's currently in the buffer to the serial line.
 */
void print_buffer(void);

/*****************************************************************************
 * Functions to handle terminal io.
 ****************************************************************************/

void initialise_terminal(void);

void cursor_to_top_left(void);

#endif /* IR_H_ */
