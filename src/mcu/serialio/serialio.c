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
    while (1) {
    /*
     * Do a loop back.
     */
        USART_put_char(USART_get_char());
    }
    return 0;
}

void USART_init(uint32_t ubrr) {
    /*
     * Set the baud rate.
     */
    UBRR0H = (uint8_t) (ubrr >> 8);
    UBRR0L = (uint8_t) (ubrr);

    /*
     * Enable TX and RX and enable interrupt on receive.
     */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /*
     * Set frame format: 8 data, 2 stop bits, even parity.
     */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

/*
 * We're reading characters sequentially from our input buffer.
 * We just have to hope we're not writing faster than we're reading,
 * or that the input buffer is large enough to hold the data long enough
 * to be read through.
 */
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
