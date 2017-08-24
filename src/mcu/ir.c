/*
 * Implements the NEC IR communication protocol.
 * Written by Huy Nguyen, adapted from Dave Jone's IR tutorial.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void init_carrier(void);
void enable_carrier(void);
void IR_carrier(uint16_t IRTimeMicroSeconds);
void send_code(uint32_t IRcode);
void send_1(void);
void send_0(void);

/*
 * A test code to send.
 */
uint32_t IRcode = 0b11000001110001111100000000111111;

/*
 * Each pulse is 562us long.
 */
uint16_t bitTime = 562;

/*
 * Main loop.
 */
int main(void) {
	while(1) {
		init_carrier();
		send_code(IRcode);
		_delay_ms(500);
	}
	return 0;
}

/*
 * Set the data direction for the IR IO and turn it off. 
 */
void init_carrier(void) {
	DDRD = 0xFF;
	PORTD = 0x00;
}

/*
 * We're sending a 38kHz carrier wave.
 * The delay values were manually tuned by inspection of oscilloscope.
 */
void enable_carrier(void) {
	PORTD = 0xFF;
	_delay_us(12);
	PORTD = 0x00;
	_delay_us(12);
}

/*
 * Enable the carrier for a certain amount of microseconds.
 */
void IR_carrier(uint16_t IRTimeMicroSeconds) {
	int i;
	/*
	 * Divide by 26 because that is the period of the 38kHz carrier.
	 */
	for (i = 0; i < IRTimeMicroSeconds/26; i++) {
		enable_carrier();
	}
}

/*
 * Send a 0 bit on the carrier wave.
 */
void send_0(void) {
	IR_carrier(bitTime);
	/*
	 * The rest time for a 0 bit is 560us
	 */
	_delay_us(560);
}

/*
 * Send a 1 bit on the carrier wave.
 */
void send_1(void) {
 	IR_carrier(bitTime);
	/*
	 * The rest time for a 1 bit is 1690us
	 */
	_delay_us(1690);
}

void send_code(uint32_t IRcode) {
	/*
	 * The NEC protocol begins with a 9ms start burst.
	 */
 	IR_carrier(9000);
	/*
	 * 4.5ms silence follows.
	 */
 	_delay_us(4500);
	int i;
	for (i = 0; i < 32; i++) {
		/*
		 * Get the MSB, NEC protocol sends MSB first.
		 */
		if (IRcode & 0x80000000) {
			send_1();
		} else {
			send_0();
		}
		/*
		 * Get the next bit to send.
		 */
		IRcode <<= 1;
	}
}
