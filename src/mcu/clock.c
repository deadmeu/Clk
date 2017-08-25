/*
 * clock.c
 *
 * Written by Alex Subaric.
 */

#include <stdint.h>

#include "clock.h"
#include "ledarray.h"

#define RING_LEDS 4
#define GRID_LEDS 16

struct cRGB ledring[RING_LEDS];
struct cRGB ledgrid[GRID_LEDS];

uint8_t is_splash;

void init_clock(void) {
  init_ledarrays();
  

  is_splash = 1;
}

// Setup an LED array
void init_ledarrays(void) {
  ledarray_clear(ledring, RING_LEDS);
  ledarray_clear(ledgrid, GRID_LEDS);
}

void splash_off(void) {
  is_splash = 0;
}
