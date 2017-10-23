/*
 * timer.c
 *
 * Written by Alex Subaric and Huy Nguyen.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer.h"

// The internal clock tick count, gets incremented every millisecond. 
// This counter will overflow after 49.7 days of continuous incrementing.
static volatile uint32_t clock_ticks;

// Setup timer0 to fire an interrupt every millisecond.
// Clock is divided by 64, and counter is set to 124.
// Starting the counter from 0 yields:
//      8,000,000 / (64 * 125) = 1000 Hz
//      Therefore, an interrupt every 1 ms @ 16 MHz
void init_timer0(void) {
    // Disable interrupts
    cli();

    // Reset the clock tick count
    clock_ticks = 0L;
    
    // Clear the counter value
    TCNT0 = 0;

    // Set the output compare value to be 124
    OCR0A = 124;

    // Turn on Clear On Compare match (CTC) mode
    TCCR0A = (1 << WGM01);

    // Set CS02, CS01, CS00 bits for 64 prescaler
    TCCR0B = (1 << CS01) | (1 << CS00);

    // Enable interrupt on output compare match
    TIMSK0 |= (1 << OCIE0A);

    // Ensure the interrupt flag is cleared by setting it to 1
    TIFR0 &= (1 << OCF0A);

    // Reenable interrupts
    sei();
}

/*
Using timer 1, we're going to output PWM on OC1A to play sound.
We're using toggle on output compare with no prescaling.
*/
void init_timer1(void) {
    // Disable interrupts
    cli();

    /*
	Using OC1A, so B1 needs to be output.
	*/
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << CS10) | (1 << WGM12);
    TCNT1   = (0x00);
    
    // Reenable interrupts
    sei();
}

// Returns the clock ticks counter
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
