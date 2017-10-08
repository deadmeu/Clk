#include "serialio.h"

void USART_init(uint32_t ubrr) {
    /*
     * Set the baud rate.
     */
    UBRR0H = (uint8_t) (ubrr >> 8);
    UBRR0L = (uint8_t) (ubrr);

    /*
     * Enable TX and RX.
     */
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /*
     * Set frame format: 8 data, 2 stop bits.
     */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
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
