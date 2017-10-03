/*
 * ledarray.h
 *
 * Written by Alex Subaric.
 */ 

#ifndef LEDARRAY_H_
#define LEDARRAY_H_

#include <stdint.h>

#include "light_ws2812.h"
#include "pixel_colour.h"

typedef enum {
    RING_PIN = 0,
    GRID_PIN = 1
} pindef_t;

void enable_leds(struct cRGB *led_array, uint8_t size, pindef_t pin_type);
void update_pixel_col(struct cRGB *pixel, pcol_t col);
void update_pixel_rgb(struct cRGB *pixel, uint8_t r, uint8_t g, uint8_t b);
void led_array_clear(struct cRGB *led_array, uint8_t size);
void opacity_array_clear(uint8_t *opacity_array, uint8_t size);

#endif /* LEDARRAY_H_ */
