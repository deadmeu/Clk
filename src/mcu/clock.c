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

static void increment_hours(void);
static void increment_minutes(void);
static void init_ledarrays(void);

struct cRGB ledring[RING_LEDS];
struct cRGB ledgrid[GRID_LEDS];

uint8_t is_splash;
uint8_t seconds; // current seconds counter
uint8_t minutes; // current minutes counter
uint8_t hours; // current hours counter



void init_clock(void) {
    init_ledarrays();
  
    seconds = 0;
    minutes = 0;
    hours = 0;
    is_splash = 1;
}

void splash_off(void) {
    is_splash = 0;
}


// TODO: should interrupts be disabled before incrementing this stuff?
void tick_clock(void) {
    if (seconds == 59) {
        // 59 seconds have already passed, now entering the 60th second so
        // reset the counter and increment the minutes counter by one.
        seconds = 0;
        increment_minutes();
    } else {
        seconds++;
    }
}

// sets the clock seconds, minutes, and hours counters to the supplied time in seconds.
void set_time(uint32_t time) {
    if (!(time >= 0L && time <= 86399L)) return;  // time (in seconds) is not between 00:00:00 and 24:00:00
    hours = time / 3600;
    minutes = (time - 3600 * hours) / 60;
    seconds = (time - 3600 * hours) - 60 * minutes;
}

void update_display(void) {
    
}

static void increment_hours(void) {
    if (hours == 23) {
        // 23 hours have already passed, now entering the 24th hour so
        // reset the counter.
        hours = 0;
    } else {
        hours++;
    }
}

static void increment_minutes(void) {
    if (minutes == 59) {
        // 59 minutes have already passed, now entering the 60th minute so
        // reset the counter and increment the hours counter by one.
        minutes = 0;
        increment_hours();
    } else {
        minutes++;
    }
}

// Setup an LED array with 0's for r, g, b
static void init_ledarrays(void) {
    ledarray_clear(ledring, RING_LEDS);
    ledarray_clear(ledgrid, GRID_LEDS);
}