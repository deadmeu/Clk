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
 * Return the next character in the serial buffer.
 */
uint8_t USART_get_char(void);

/*
 * Send a character from the output buffer to the serial stream.
 */
void USART_put_char(uint8_t data);

void update_leds(uint8_t val);

int isNthBitSet(unsigned char c, int n);

#endif
