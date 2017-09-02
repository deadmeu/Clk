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

void enable_leds(struct cRGB *led_array, uint8_t size);
void update_pixel(struct cRGB *pixel, uint8_t r, uint8_t g, uint8_t b);
void led_array_clear(struct cRGB *led_array, uint8_t size);
void opacity_array_clear(uint8_t *opacity_array, uint8_t size);

#endif /* LEDARRAY_H_ */