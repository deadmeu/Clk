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
#define F_CPU 8000000UL
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
#define C7 2093.00

void play_note(int note);
void delay_note(int duration);
void setup_sound(void);
void play_scale(void);
void play_octaves(void);
void play_big_ben(void);
void play_alarm(void);
void silence(int duration);

int main(void)
{
	setup_sound();
    /* Replace with your application code */
    while (1) {
		play_alarm();
    }
}

/*
Using timer 1, we're going to output PWM on OC1A to play sound.
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
/*
A value of 2000000 seems to give clear sounding notes, chosen by trial and error.
*/
	OCR1A = (2000000 / note * 2) - 1;
}

void delay_note(int duration) {
	while (duration--) {
		_delay_ms(1);
	}
}

void silence(int duration) {
	DDRB &= ~(1 << PORTB1);
	delay_note(duration);
	DDRB |= 1 << PORTB1;
}

void play_scale(void) {
	int notes[] = {C5,D5,E5,F5,G5,A5,B5,C6,D6,E6,F6,G6,A6,B6,C7};
	int i;
	for (i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
		play_note(notes[i]);
		_delay_ms(2000);
	}
	for (i = sizeof(notes) / sizeof(notes[0]) - 1; i > 0; i--){
		play_note(notes[i]);
		_delay_ms(2000);
	}
}

void play_octaves(void) {
	int notes[] = {C5,C6,D5,D6,E5,E6,F5,F6,G5,G6,A5,A6,B5,B6,C6,C7};
	int i;
	for (i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
		play_note(notes[i]);
		_delay_ms(2000);
	}
}

void play_big_ben(void) {
/*
Play a note for its corresponding duration
*/
	int notesAndBreaks[] = {E5,4000, 
			C5,4000, 
			D5,4000,
			G5,12000,
			G5,4000,
			D5,4000,
			E5,4000,
			C5,12000,
			E5,4000,
			D5,4000,
			C5,4000,
			G5,12000,
			G5,4000,
			D5,4000,
			E5,4000,
			C5,12000};
	int i;
	for (i = 0; i < sizeof(notesAndBreaks) / sizeof(notesAndBreaks[0]); i+=2) {
		play_note(notesAndBreaks[i]);
		delay_note(notesAndBreaks[i+1]);
	}
}

void play_alarm(void) {
	int notesAndBreaks[] = {C5,1000,
	E5,1000,
	G5,2000,
	E5,1000,
	G5,1000,
	C6,6000,
	C5,1000,
	E5,1000,
	G5,2000,
	E5,1000,
	G5,1000,
	C6,8000};
	int i;
	for (i = 0; i < sizeof(notesAndBreaks) / sizeof(notesAndBreaks[0]); i+=2) {
		play_note(notesAndBreaks[i]);
		delay_note(notesAndBreaks[i+1]);
	}
	silence(8000);
}
