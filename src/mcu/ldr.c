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

void init_ldr(void) {
    // Set AVCC as reference
    ADMUX = (1 << REFS0);

    // Enable ADC
    ADCSRA = _BV(ADEN);
}

uint8_t get_ldr_opacity(void) {
    uint16_t result;
    uint16_t adc_value = adc_read(ADC_PIN);

    // Clamp the adc values between ADC_MIN and ADC_MAX
    result = (adc_value > ADC_MAX) ? ADC_MAX : adc_value;
    result = (adc_value < ADC_MIN) ? ADC_MIN : adc_value;
	
    // Convert result from ADC value range to opacity value range
    result = (((result - ADC_MIN) * OPACITY_RANGE) / ADC_RANGE) + MIN_OPACITY;

	
	// if (result < 10) {
	// 	set_meridiem_colours(1, 255,0,0);
	// } else if (result >95 ) {
	// 	set_meridiem_colours(1, 0,255,0);
	// } else {
	// 	set_meridiem_colours(1, 0, 0, 255);
	// }

    return (uint8_t) result;
}

static uint16_t adc_read(uint8_t pin) {
    // Disable interrupts to ensure safe copy. Interrupts are only
    // reenabled if they were enabled from the start.
    uint8_t interrupts_enabled = bit_is_set(SREG, SREG_I);
    
    // Disable interrupts
    cli();

    ADMUX &= 0xF0;
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
