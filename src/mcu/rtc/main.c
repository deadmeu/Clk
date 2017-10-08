#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "i2c_lib/i2c_master.h"
#include "serialio/serialio.h"

#define MCP7940N_WRITE  0b11011110
#define MCP7940N_READ   0b11011111

void test_osc(void);
void set_seconds(void);
int read_seconds(void);

int main() {
    i2c_init();
    USART_init(UBRR);
    set_seconds();
    while (1) {
        int seconds = read_seconds();
        char output[255];
        sprintf(output, "%d", seconds);
        USART_puts(output);
        _delay_ms(1000);
    }
    return 0;
}

void test_osc(void) {
    i2c_start(MCP7940N_WRITE);
    i2c_write(0x03);    // Set the pointer to the WKDAY register.
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t ret = i2c_read_ack();
    i2c_stop();
    ret &= 0b00100000;
    if (ret == 0) {
        while(1) {
            USART_puts("Oscillator has stopped.");
        }
    }
}

void set_seconds(void) {
    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the seconds register.
     */
    i2c_write(0x00);
                            // Enable the oscillator.
                            //      // Set the seconds tens.
                            //      //              // Set the seconds ones.
    uint8_t startSeconds =  0x80    | (0b01010000)  | (0b00001001);
    i2c_write(startSeconds);    
    i2c_stop();
}

int read_seconds(void) {
    i2c_start(MCP7940N_WRITE);
    /*
     * Set the pointer to the seconds register.
     */
    i2c_write(0x00);
    i2c_stop();

    i2c_start(MCP7940N_READ);
    uint8_t seconds = i2c_read_ack();
    i2c_stop();

    int secOnes = seconds & 0b00001111;
    int secTens = (seconds & 0b01110000) >> 4;

    return secOnes + secTens * 10;
}
