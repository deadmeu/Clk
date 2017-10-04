#include "serialio.h"
#include "light_ws2812.h"

/*****************************************************************************
 * Constants for use in serial communication.
 ****************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000L
#endif

#define BAUD 9600
#define UBRR (F_CPU/16/BAUD) - 1
#define GRID_LEDS 16

struct cRGB led_grid[GRID_LEDS];

int main(void) {
    //USART_init(UBRR);
    while(1) {
        update_leds(10);
        ws2812_setleds_pin(led_grid, GRID_LEDS, _BV(5));
    }
    return 0;
}

void update_leds(uint8_t val) {
    for (uint8_t i = 0; i < GRID_LEDS; i++) {
        if (isNthBitSet(val, i)) {
            led_grid[i].r = 255; led_grid[i].g = 0; led_grid[i].b = 0;
        } else {
            led_grid[i].r = 0; led_grid[i].g = 0; led_grid[i].b = 0;            
        }
    }
}

int isNthBitSet (unsigned char c, int n) {
    static unsigned char mask[] = {128, 64, 32, 16, 8, 4, 2, 1};
    return ((c & mask[n]) != 0);
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
}
