/*
 * timer.c
 *
 * Written by Alex Subaric, adapted from Peter Sutton's timer0.c.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

// The internal clock tick count, gets incremented every millisecond
static volatile uint32_t clock_ticks;

// Setup timer0 to fire an interrupt every millisecond.
// Clock is divided by 64, and counter is set to 248.
// Starting the counter from 0 yields:
//      16,000,000 / (64 * 249) = 1000 Hz
//      Therefore, an interrupt every 1 ms @ 16 MHz
void init_timer0(void) {
    // Disable interrupts
    cli();

    // Reset the clock tick count
    clock_ticks = 0L;
    
    // Clear the counter value
    TCNT0 = 0;

    // Set the output compare value to be 248
    OCR0A = 248;

    // Turn on Clear On Compare match (CTC) mode
    TCCR0A = (1 << WGM01);

    // Set CS02, CS01, CS00 bits for 64 prescaler
    TCCR0B = (0 << CS02) | (1 << CS01) | (1 << CS00);

    // Enable interrupt on output compare match
    TIMSK0 |= (1 << OCIE0A);

    // Ensure the interrupt flag is cleared by setting it to 1
    TIFR0 &= (1 << OCF0A);

    // Reenable interrupts
    sei();
}

uint32_t get_clock_ticks(void) {
    uint32_t result;

    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);

    // Disable interrupts
    cli();

    // Copy the current number of clock ticks
    result = clock_ticks;

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();

    return result;
}

// Interrupt handler, which fires every millisecond
ISR(TIMER0_COMPA_vect) {
    // Increment the clock tick count
    clock_ticks++;
}