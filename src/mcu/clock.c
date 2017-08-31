/*
 * clock.c
 *
 * Written by Alex Subaric.
 */

#include <stdint.h>

#include "clock.h"
#include "ledarray.h"
#include "pixel_colour.h"

#define RING_LEDS   4
#define GRID_LEDS   16
#define MAX_TIME    86399L              // 23:59:59 converted to seconds. L for long (32 bit) type

#define HOURS        time / 3600
#define MINUTES     (time - 3600 * HOURS) / 60
#define SECONDS     (time - 3600 * HOURS) - 60 * MINUTES
#define FOURS       MINUTES % 5

// static void increment_hours(void);
// static void increment_minutes(void);
static void init_leds(void);

// LED arrays
struct cRGB led_ring[RING_LEDS];
struct cRGB led_grid[GRID_LEDS];

// RGB arrays
struct cRGB rgb_ring[RING_LEDS];
struct cRGB rgb_grid[GRID_LEDS];

// Opacity arrays
uint8_t opacity_ring[RING_LEDS];
uint8_t opacity_grid[GRID_LEDS];

                                            // [0]   [3] ~Index numbers~
                                            // {1 0 0 2}
                                            // {0 0 0 0} Grid is arranged in a 4x4 layout with minute markers
uint8_t grid_minute_states[4] = {0,  3,     // {0 0 0 0} in each corner.
                                12, 15};    // {3 0 0 4}
                                            // [12] [15] ~Index numbers~
   
uint32_t time;
uint32_t last_time;
uint8_t splash_flag;
uint8_t animation_flag;
uint8_t hour_marker_flag;
// uint8_t seconds; // current seconds counter
// uint8_t minutes; // current minutes counter
// uint8_t hours; // current hours counter
uint8_t alarm_time;



void init_clock(void) {
    init_leds();
  
    last_time = time = 0L;
    // seconds = 0;
    // minutes = 0;
    // hours = 0;
    alarm_time = 60;    // 1 minute past midnight
    splash_flag = 1;
    animation_flag = 0;
    hour_marker_flag = 1;
}

void splash_off(void) {
    splash_flag = 0;
}

// TODO: A lot of the timekeeping functionality should be in its own timekeeper.c file

// TODO: should interrupts be disabled before incrementing this stuff?
void increment_seconds(void) {
    time++;
    // if (SECONDS == 59) {
    //     // 59 seconds have already passed, now entering the 60th second so
    //     // reset the counter and increment the minutes counter by one.
    //     seconds = 0;
    //     increment_minutes();
    // } else {
    //     seconds++;
    // }
}

// TODO: probably put this in a weather.c file?
void play_weather_animation(void) {
    animation_flag = 1;
}

void stop_weather_animation(void) {
    animation_flag = 0;
}

uint8_t animation_playing(void) {
    return animation_flag;
}

// TODO: maybe put this in a timekeeper.c file? or maybe not?
uint8_t reached_new_minute(void) {
    return MINUTES % 60 == 0;
}

// sets the clock seconds, minutes, and hours counters to the supplied time in seconds.
void set_time(uint32_t new_time) {
    if (!(new_time >= 0L && new_time <= MAX_TIME)) return;  // new_time (in seconds) is not between 00:00:00 and 23:59:59 inclusive
    // hours = new_time / 3600;
    // minutes = (new_time - 3600 * hours) / 60;
    // seconds = (new_time - 3600 * hours) - 60 * minutes;
    time = new_time;
}

void show_display(void) {
    enable_leds(led_ring, RING_LEDS);
}

void toggle_hour_marker(void) {
    hour_marker_flag ^= 1;
}

void apply_opacity(void) {
    // Adjust the ring 
    for (uint8_t i = 0; i < RING_LEDS; i++) {
        // Set each pixel to (r * opacity)/100, (g * opacity)/100, (b * opacity)/100
        update_pixel(&led_ring[i], (rgb_ring[i].r * opacity_ring[i]) / 100, 
            (rgb_ring[i].g * opacity_ring[i]) / 100, (rgb_ring[i].b * opacity_ring[i]) / 100);
    }

    // Adjust the grid 
    for (uint8_t i = 0; i < GRID_LEDS; i++) {
        // Set each pixel to (r * opacity)/100, (g * opacity)/100, (b * opacity)/100
        update_pixel(&led_grid[i], (rgb_grid[i].r * opacity_grid[i]) / 100, 
            (rgb_grid[i].g * opacity_grid[i]) / 100, (rgb_grid[i].b * opacity_grid[i]) / 100);
    }
}

void update_display(void) {
    // Update entire ring
    for (uint8_t i = 0; i * 5 <= MINUTES; i++) {        // iterate over each 5 minute marker up to and including the marker
                                                        // corresponding to the current minutes.
        //if (i * (60 / RING_LEDS) <= MINUTES) {
        update_pixel(&rgb_ring[i], RED);
        //}
    }

    // Add hour marker to grid if necessary
    if (hour_marker_flag) {
        update_pixel(&rgb_ring[HOURS % RING_LEDS], GREEN);
    }

    // Update grid
    if (animation_playing()) {      // display the animation
        // TODO animation logic
    } else {                        
        // Display the fours
        if (MINUTES % 5) {          // between 1-4 minutes past a 5 minute marker
            for (uint8_t i = 0; i < FOURS; i++) {
                update_pixel(&rgb_grid[grid_minute_states[i]], RED);
            }
        } else {
            led_array_clear(&rgb_grid, GRID_LEDS);
        }
    }
}

// All of this is redundant with the new time logic
// static void increment_hours(void) {
//     if (hours == 23) {
//         // 23 hours have already passed, now entering the 24th hour so
//         // reset the counter.
//         hours = 0;
//     } else {
//         hours++;
//     }
// }

// static void increment_minutes(void) {
//     if (minutes == 59) {
//         // 59 minutes have already passed, now entering the 60th minute so
//         // reset the counter and increment the hours counter by one.
//         minutes = 0;
//         increment_hours();
//     } else {
//         minutes++;
//     }
// }

// Setup the LED arrays, clearing the r, g, b values
static void init_leds(void) {
    // Clear the LED arrays
    led_array_clear(&led_ring, RING_LEDS);
    led_array_clear(&led_grid, GRID_LEDS);
    
    // Clear the RGB arrays
    led_array_clear(&rgb_ring, RING_LEDS);
    led_array_clear(&rgb_grid, GRID_LEDS);

    // Clear the opacity arrays
    opacity_array_clear(opacity_ring, RING_LEDS);
    opacity_array_clear(opacity_grid, GRID_LEDS);
}
