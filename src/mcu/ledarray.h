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

void ledarray_update_pixel(struct cRGB *pixel, uint8_t r, uint8_t g, uint8_t b);
void ledarray_clear(struct cRGB ledarray[], uint8_t size);

#endif /* LEDARRAY_H_ */