/*
 * animation.h
 *
 * Written by Alex Subaric.
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <stdint.h>
#include "unique_types.h"
#include "pixel_colour.h"

void reset_frame_count(void);

uint8_t incr_frame_count(void);

pcol_t get_frame_pixel(wtype_t weather, uint8_t i);

#endif /* ANIMATION_H_ */
