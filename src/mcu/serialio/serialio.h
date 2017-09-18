#ifndef SERIALIO_H_
#define SERIAL_H_

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*****************************************************************************
 * Function prototypes.
 ****************************************************************************/

/*
 * This function is called as 
 * USART_init(UBRR);
 */
void USART_init(uint32_t ubrr);

/*
 * Empties the USART data register.
 * Not sure when we might need this.
 */
void USART_flush(void);

/*
 * Return the next character in the serial buffer.
 */
uint8_t USART_get_char(void);

#endif