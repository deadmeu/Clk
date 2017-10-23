/*
 * ldr.c
 *
 * Written by Alex Subaric.
 */

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "ldr.h"
#include "clock.h"

#define ADC_PIN            0    // PC0
#define ADC_MIN          100
#define ADC_MAX         1000

#define ADC_RANGE       (ADC_MAX - ADC_MIN)
#define OPACITY_RANGE   (MAX_OPACITY - MIN_OPACITY)

static uint16_t adc_read(uint8_t pin);

// Initialise the LDR
void init_ldr(void) {
    // Set AVCC as reference
    ADMUX = (1 << REFS0);

    // Enable ADC
    ADCSRA = _BV(ADEN);
}

// Returns the ldr opacity value
uint8_t get_ldr_opacity(void) {
    uint16_t result;
    uint16_t adc_value = adc_read(ADC_PIN);

    // Clamp the adc values between ADC_MIN and ADC_MAX
    result = (adc_value > ADC_MAX) ? ADC_MAX : adc_value;
    result = (adc_value < ADC_MIN) ? ADC_MIN : adc_value;

    // Convert result from ADC value range to opacity value range
    result = (result - ADC_MIN) * (OPACITY_RANGE*100 / ADC_RANGE) / 100 * AMBIENT_MULTIPLIER + MIN_OPACITY;    

    return (uint8_t) result;
}

// Reads a value from ADC
static uint16_t adc_read(uint8_t pin) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);
    
    // Disable interrupts
    cli();

    ADMUX &= 0xF8;
    ADMUX |= pin;

    // Start the ADC conversion
    ADCSRA |= _BV(ADSC);

    // Wait until the conversion is complete
    while (ADCSRA & _BV(ADSC));

    // Renable interrupts if they were enabled previously
    if (interrupts_enabled) sei();

    // Return the converted value
    return ADC;
}
