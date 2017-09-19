/*
 * spi.c
 *
 * Written by Alex Subaric.
 */ 

#include <avr/io.h>
#include "spi.h"

void spi_setup_master(uint8_t clockdivider) {
	// SPI communication as master
    // Set SS, MOSI, SCK pins as outputs
    DDRB |= (1<<3)|(1<<4)|(1<<6);

    // Set up communication with LED array

    // Set Slave Select (SS) line high
    PORTB |= (1<<3)

    // Set up the SPI control registers SPCR and SPSR:
	// - SPE bit = 1 (SPI is enabled)
	// - MSTR bit = 1 (Master Mode)
	SPCR0 = (1<<SPE0)|(1<<MSTR0);

    // Set SPR0 and SPR1 bits in SPCR and SPI2X bit in SPSR
	// based on the given clock divider
	// Invalid values default to the slowest speed
	// We consider each bit in turn
	switch(clockdivider) {
		case 2:
		case 8:
		case 32:
			SPSR0 = (1<<SPI2X0);
			break;
		default:
			SPSR0 = 0;
			break;
	}
	switch(clockdivider) {
		case 128:
			SPCR0 |= (1<<SPR00);
			// Note this flows through to the next code block
		case 32:
		case 64:
			SPCR0 |= (1<<SPR10);
			break;
		case 8:
		case 16:
			SPCR0 |= (1<<SPR00);
			break;
	}

    // Take SS (slave select) line low
	PORTB &= ~(1<<3);
}

uint8_t spi_send_byte(uint8_t byte) {
	// Write out the byte to the SPDR register. This will initiate
	// the transfer. We then wait until the most significant byte of
	// SPSR (SPIF bit) is set - this indicates that the transfer is
	// complete. (The final read of SPSR followed by a read of SPDR
	// will cause the SPIF bit to be reset to 0. See page 173 of the 
	// ATmega324A datasheet.)
	SPDR0 = byte;
	while((SPSR0 & (1<<SPIF0)) == 0) {
		; // wait
	}
	return SPDR0;
}