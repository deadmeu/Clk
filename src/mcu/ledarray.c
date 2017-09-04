/*
 * ledarray.c
 *
 * Written by Alex Subaric.
 */ 

#include <avr/io.h>
#include "ledarray.h"
#include "light_ws2812.h"
#include "pixel_colour.h"

void enable_leds(struct cRGB *led_array, uint8_t size) {
    ws2812_setleds(led_array, size);
}

void update_pixel(struct cRGB *pixel, uint8_t r, uint8_t g, uint8_t b) {
    // r, g, b values must be between 0 and 255
    if (!(r >= 0 && g >= 0 && b >= 0 && r <= 255 && g <= 255 && b <= 255)) return;
    pixel->r = r;
    pixel->g = g;
    pixel->b = b;
}

void led_array_clear(struct cRGB *led_array, uint8_t size) {
    // Set each pixel to black
    for (uint8_t i = 0; i < size; i++) {
        led_array[i].r = 0;
        led_array[i].g = 0;
        led_array[i].b = 0;
    }
    // memset(led_array, 0, size * sizeof(led_array));
}

void opacity_array_clear(uint8_t *opacity_array, uint8_t size) {
    // Set each element to 100 (full brightness)
    for (uint8_t i = 0; i < size; i++) {
        opacity_array[i] = 100;
    }
}
