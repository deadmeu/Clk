/*
 * ledarray.c
 *
 * Written by Alex Subaric.
 */ 

#include <avr/io.h>

#include "ledarray.h"

#define NUM_LEDS 4

struct cRGB ledarray[NUM_LEDS];

void ledarray_setup(void) {
	// Setup the led array
}

void ledarray_update_pixel(uint8_t num, struct cRGB pixel) {
	ledarray[num] = pixel;
}

void ledarray_clear(void) {
	for (int i = 0; i < NUM_LEDS; i++) {
		// Set the r, g, b values of each led to 0
		//ledarray[i].r, ledarray[i].g, ledarray[i].b = 0;
	}
}