#include "serialio.h"

/*****************************************************************************
 * Constants for use in serial communication.
 ****************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define BAUD 9600
#define UBRR (F_CPU/16/BAUD) - 1

int main(void) {
    USART_init(UBRR);
    while(1) {
    }
    return 0;
}

/*****************************************************************************
 * Functions to enable serial communication.
 ****************************************************************************/

void USART_init(uint32_t ubrr) {
    /*
     * Set the baud rate.
     */
    UBRR0H = (uint8_t) (ubrr >> 8);
    UBRR0L = (uint8_t) (ubrr);

    /*
     * Enable TX and RX and enable interrupt on receive.
     * Enable interrupt on serial read.
     */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    /*
     * Set frame format: 8 data, 2 stop bits.
     */
    UCSR0C = (1 << USBS0) | (1 << UCSZ00) | (1 << UCSZ01);
    
    sei();
}

uint8_t USART_get_char(void) {
    while (!(UCSR0A & (1 << RXC0))) {
    /*
     * Wait for data to be received.
     */
    }
    return UDR0;
}

void USART_put_char(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0))) {
    /*
     * Wait for the data register to empty.
     */
    }
    UDR0 = data;
}

ISR(USART_RX_vect) {
    uint8_t c = UDR0;
    c += 48;
    UDR0 = c;
}
