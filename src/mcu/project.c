/*
 * project.c
 *
 * Written by Alex Subaric.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "clock.h"
#include "ir.h"
#include "ldr.h"
#include "ledarray.h"
#include "splash.h"
#include "timer.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

#define DISPLAY_UPDATE_DELAY 600

void initialise_hardware(void);
void initialise_clock(void);
void run_clock(void);
void reset_clock(void);

int main(void) {
    initialise_hardware();
    splash_screen();
    splash_off();

    while(1) {
        initialise_clock();
        run_clock();
        reset_clock();
    }
}

void initialise_hardware(void) {
    // Setup the main timer, providing an interrupt every millisecond
    init_timer0();

    // Turn on global interrupts
    sei();

}

void initialise_clock(void) {
    init_clock();
}

void run_clock(void) {
    uint32_t last_clock_tick_time;
    uint32_t last_display_time;

    last_clock_tick_time = last_display_time = get_clock_ticks();

    while(1) {
        if (get_clock_ticks() - last_display_time >= DISPLAY_UPDATE_DELAY) {
            // Half a second has passed since the last display tick, so update the display
            update_display();
            last_display_time = get_clock_ticks();
        }
        if (get_clock_ticks() - last_clock_tick_time >= 1000) {
            // One second has passed since the last clock tick, so increment the time by 1 second
            tick_clock();

        }
    }
}

void reset_clock(void) {
    while (1) { ; } //hang the system
}
