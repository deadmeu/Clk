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

// example usage
// uint8_t main(void)
// {
// 	setup_sound();
//     play_alarm();
//     /* Replace with your application code */
//     while (1) {
//     }
// }

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
    // A value of 2000000 seems to give clear sounding notes, chosen through trial and error.
    OCR1A = (2000000 / note * 2) - 1;
}

// void delay_note(uint8_t duration) {
//     uint32_t start_time = get_clock_ticks();
//     while (get_clock_ticks() - start_time < duration) {
//         // Do nothing
//     }
// }

// /* Silences sound output for the specified duration. */
// void silence(uint8_t duration) {
//     sound_off();
//     delay_note(duration);
//     sound_on();
// }

// void play_scale(void) {
//     sound_on();
//     uint8_t notes[] = {C5, D5, E5, F5, G5, A5, B5, C6,
//                        D6, E6, F6, G6, A6, B6, C7};
//     for (uint8_t i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
//         play_note(notes[i]);
//         _delay_ms(200);
//     }
//     for (uint8_t i = sizeof(notes) / sizeof(notes[0]) - 1; i > 0; i--){
//         play_note(notes[i]);
//         _delay_ms(200);
//     }
//     sound_off();
// }

// void play_octaves(void) {
//     sound_on();
//     uint8_t notes[] = {C5, C6, D5, D6, E5, E6, F5, F6,
//                        G5, G6, A5, A6, B5, B6, C6, C7};
//     uint8_t i;
//     for (i = 0; i < sizeof(notes) / sizeof(notes[0]); i++){
//         play_note(notes[i]);
//         _delay_ms(200);
//     }
//     sound_off();
// }

// void play_big_ben(void) {
//     sound_on();
//     uint8_t notes_and_breaks[] = {E5,400, C5,400, D5,400, G5,1200, 
//                                   G5,400, D5,400, E5,400, C5,1200, 
//                                   E5,400, D5,400, C5,400, G5,1200, 
//                                   G5,400, D5,400, E5,400, C5,1200};
//     for (uint8_t i = 0; i < sizeof(notes_and_breaks) / sizeof(notes_and_breaks[0]); i += 2) {
//         // Play a note for its corresponding duration
//         play_note(notes_and_breaks[i]);
//         delay_note(notes_and_breaks[i + 1]);
//     }
//     sound_off();
// }

// void play_alarm(void) {
//     sound_on();
//     uint8_t notes_and_breaks[] = {
//     C5,100,
//     E5,100,
//     G5,200,
//     E5,100,
//     G5,100,
//     C6,600,
//     //break
//     C5,100,
//     E5,100,
//     G5,200,
//     E5,100,
//     G5,100,
//     C6,800
//     };
//     uint8_t i;
//     for (i = 0; i < sizeof(notes_and_breaks) / sizeof(notes_and_breaks[0]); i+=2) {
//         play_note(notes_and_breaks[i]);
//         delay_note(notes_and_breaks[i+1]);
//     }
//     sound_off();
// }

void update_sound(void) {
    if (alarm_is_playing()) {
        sound_on();
        
        // Check if its time to update the note.
        if (get_clock_ticks() - start_delay_time >= delay_duration) {
            // Not a delay note - so play the note.
            play_note(sheet_alarm[note_index++ % (sizeof(sheet_alarm) / sizeof(sheet_alarm[0]))]);

            // Update the delay time.
            delay_duration = sheet_alarm[note_index++ % (sizeof(sheet_alarm) / sizeof(sheet_alarm[0]))];
            start_delay_time = get_clock_ticks();
        }
    } else {
        sound_off();
    }
}

// void play_alarm(void) {
//     play_sheet(sheet_alarm, sizeof(sheet_alarm) / sizeof(sheet_alarm[0]));
// }

// /* Plays the supplied sheet of notes and delays. */
// void play_sheet(uint8_t *sheet, uint8_t size) {
//     sound_on();
//     for (uint8_t i = 0; i < size; i += 2) {
//         play_note(sheet[i]);
//         delay_note(sheet[i + 1]);
//     }
//     sound_off();
// }
