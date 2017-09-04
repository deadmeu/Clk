/*
 * notes.c
 *
 * Created: 4/09/2017 1:43:03 PM
 * Author : chrom
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef F_CPU 
#define F_CPU 1000000UL
#endif

/*
Here are the note frequencies
*/
#define C5 523.25
#define D5 587.33
#define E5 659.25
#define F5 698.46
#define G5 783.99	
#define A5 880.00
#define B5 987.77
#define C6 1046.50
#define D6 1174.66
#define E6 1318.51
#define F6 1396.91
#define G6 1567.98
#define A6 1760.00
#define B6 1975.53

void play_note(int note);
void setup_sound(void);

int main(void)
{
	setup_sound();
    /* Replace with your application code */
    while (1) {
		play_note(C5);
		_delay_ms(1000);
		play_note(E5);
		_delay_ms(1000);
		play_note(G5);
		_delay_ms(1000);
    }
}

/*
Using timer 0, we're going to output PWM on OC1A to play sound.
We're using toggle on output compare with no prescaling.
*/
void setup_sound(void) {
	/*
	Using OC1A, so B1 needs to be output.
	*/
	DDRB |= 1 << PORTB1;
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << CS10) | (1 << WGM12);
	TCNT1 = 0;
}

/*
The formula for outputting a frequency is
OCR1A = (f_clk / f_out * 2 * prescale) - 1.
*/
void play_note(int note) {
	OCR1A = (F_CPU / note * 2) - 1;
}