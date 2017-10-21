/*
 * sound.c
 *
 * Written by Huy Nguyen and Alex Subaric.
 */ 

#include <stdint.h> 
#include <avr/io.h>
#include <util/delay.h>

#include "clock.h"
#include "timer.h"

/* Note frequencies */
#define C5  523
#define D5  587
#define E5  659
#define F5  698
#define G5  783	
#define A5  880
#define B5  987
#define C6 1046
#define D6 1174
#define E6 1318
#define F6 1396
#define G6 1567
#define A6 1760
#define B6 1975
#define C7 2093

/* Sound sheets */
uint16_t sheet_bigben[] = {E5,400, C5,400, D5,400, G5,1200, 
                           G5,400, D5,400, E5,400, C5,1200, 
                           E5,400, D5,400, C5,400, G5,1200, 
                           G5,400, D5,400, E5,400, C5,1200};

uint16_t sheet_scale[] = {C5,200, D5,200, E5,200, F5,200, 
                          G5,200, A5,200, B5,200, C6,200, 
                          D6,200, E6,200, F6,200, G6,200, 
                          A6,200, B6,200, C7,200,
                          // Back down from C7 to C5.
                          C7,200, B6,200, A6,200, G6,200, 
                          F6,200, E6,200, D6,200, C6,200,  
                          B5,200, A5,200, G5,200, F5,200,  
                          E5,200, D5,200, C5,200};

uint16_t sheet_octave[] = {C5,200, C6,200, D5,200, D6,200, 
                           E5,200, E6,200, F5,200, F6,200, 
                           G5,200, G6,200, A5,200, A6,200, 
                           B5,200, B6,200, C6,200, C7,200};

uint16_t sheet_alarm[] = {C5,100, E5,100, G5,200, E5,100, 
                          G5,100, C6,600, C5,100, E5,100, 
                          G5,200, E5,100, G5,100, C6,800};

/* Sound control variables */
uint32_t start_delay_time;
uint16_t delay_duration;
uint8_t note_index;

void setup_sound(void) {
    note_index = 0;
    delay_duration = 0;
    start_delay_time = 0;
}

/* Enables the sound output pin. */
void sound_on(void) {
    DDRB |= (1 << PORTB1);
}

/* Disables the sound output pin. */
void sound_off(void) {
    DDRB &= ~(1 << PORTB1);
}

/* Plays the given note. */
void play_note(uint16_t note) {
    // The formula for outputting a frequency is
    // OCR1A = (f_clk / f_out * 2 * prescale) - 1.
    // A value of 2000000 seems to give clear sounding notes, 
    // chosen through trial and error.
    OCR1A = (2000000 / note * 2) - 1;
}

void update_sound(void) {
    if (alarm_is_playing()) {
        sound_on();
        
        // Check if it's time to update the note.
        if (get_clock_ticks() - start_delay_time >= delay_duration) {
            // Not a delay note - so play the note.
            play_note(sheet_bigben[note_index++ % (sizeof(sheet_bigben) 
                                                    / sizeof(sheet_bigben[0]))]);

            // Update the delay time.
            delay_duration = sheet_bigben[note_index++ % (sizeof(sheet_bigben) 
                                                    / sizeof(sheet_bigben[0]))];
            start_delay_time = get_clock_ticks();
        }
    } else if (splash_playing()) {
        sound_on();
        
        // Check if it's time to update the note.
        if (get_clock_ticks() - start_delay_time >= delay_duration) {
            // Not a delay note - so play the note.
            play_note(sheet_alarm[note_index++ % (sizeof(sheet_alarm) 
                                                    / sizeof(sheet_alarm[0]))]);

            // Update the delay time.
            delay_duration = sheet_alarm[note_index++ % (sizeof(sheet_alarm) 
                                                    / sizeof(sheet_alarm[0]))];
            start_delay_time = get_clock_ticks();
        }
    } else {
        sound_off();
    }
}
