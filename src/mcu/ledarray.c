/*
 * ledarray.c
 *
 * Written by Alex Subaric.
 */ 

#include <avr/io.h>
#include "ledarray.h"
#include "light_ws2812.h"
#include "pixel_colour.h"


void update_pixel(struct cRGB *pixel, uint8_t r, uint8_t g, uint8_t b) {
    // r, g, b values must be between 0 and 255
    if (!(r >= 0 && g >= 0 && b >= 0 && r <= 255 && g <= 255 && b <= 255)) return;
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
}

void ledarray_clear(struct cRGB *ledarray, uint8_t size) {
    // Set each pixel to black
    for (uint8_t i = 0; i < size; i++) {
        update_pixel(&ledarray[i], BLACK);
    }
}
