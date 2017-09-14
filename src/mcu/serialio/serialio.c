#include "serialio.h"

/*****************************************************************************
 * Constants for use in serial communication.
 ****************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define BAUD 9600
#define UBRR (F_CPU/16/BAUD) - 1

#define USART_RX_BUFFER_SIZE 256

/*****************************************************************************
 * Data structure/s and marker/s for serial communication.
 ****************************************************************************/

static uint8_t USART_rxBuffer[USART_RX_BUFFER_SIZE];
static uint8_t USART_rxHead;
static uint8_t USART_readHead;

int main(void) {
    USART_init(UBRR);
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
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    /*
     * Set frame format: 8 data, 2 stop bits, even parity.
     */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00) | (2 < UPM00);

    /*
     * Place the buffer marker and read marker at the start.
     */
    USART_rxHead = 0;
    USART_readHead = 0;

    /*
     * This is here in case we want to routinely reset the clock in the 
     * future.
     */
    USART_flush();

    /*
     * Initialize the serial buffer.
     */
    memset(USART_rxBuffer, 0, USART_RX_BUFFER_SIZE);
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
    uint8_t interruptsWereOn = bit_is_set(SREG, SREG_I);
    cli();
    uint8_t c = USART_rxBuffer[USART_readHead];

    if (USART_readHead == USART_RX_BUFFER_SIZE - 1) {
    /*
     * If the read head reaches the end of the buffer, wrap around to the
     * start.
     */
        USART_readHead = 0;
    } else {
        USART_readHead++;
    }

    if (interruptsWereOn) {
    /*
     * Return the interrupt flag to the previous state.
     */
        sei();
    }
    return c;
}

void USART_flush(void) {
    uint8_t dummy;
    while (UCSR0A & (1 << RXC0)) {
        dummy = UDR0;
    }
}

/*
 * Interrupt handler for serial data receive complete.
 */
ISR(USART_RX_vect) {
    uint8_t data = UDR0;
    if (USART_rxHead != USART_RX_BUFFER_SIZE - 1) {
    /*
     * Update the global buffer with the new character.
     */
        USART_rxBuffer[USART_rxHead] = data;
        USART_rxHead++;
    } else {
    /*
     * If the buffer is full, we wrap around and rewrite it.
     * The current data will be lost.
     */
        USART_rxHead = 0;
    }
}
