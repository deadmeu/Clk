#ifndef SERIALIO_H_
#define SERIALIO_H_

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define BAUD 9600
#define UBRR (F_CPU/16/BAUD) - 1

/*****************************************************************************
 * Function prototypes.
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

#endif
