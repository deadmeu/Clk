/*
 * ledarray.h
 *
 * Written by Alex Subaric.
 */ 

#ifndef LEDARRAY_H_
#define LEDARRAY_H_

#include <stdint.h>

#include "light_ws2812.h"

void ledarray_setup(void);
void ledarray_update_pixel(uint8_t num, struct cRGB pixel);
void ledarray_clear(void);

#endif /* LEDARRAY_H_ */