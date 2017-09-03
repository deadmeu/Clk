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

// Other variables
uint32_t time;
uint32_t alarm_time;
uint8_t opacity_amount;

// Flags
uint8_t splash_flag;
uint8_t animation_flag;
uint8_t alarm_flag;
uint8_t hour_marker_flag;
uint8_t new_minute_flag;
uint8_t alarm_playing_flag;
uint8_t weather_set_flag;
uint8_t alarm_set_flag;
uint8_t draw_ring_flag;
uint8_t draw_grid_flag;

void init_clock(void) {
    init_leds();
    
    stop_alarm_sound();
    stop_weather_animation();

    reset_alarm_flag();
    reset_minute_flag();

    call_ring_redraw();
    call_grid_redraw();
  
    time = 67354L;       // 18:42:34
    alarm_time = 60L;    // 1 minute past midnight
    splash_flag = 1;
    hour_marker_flag = 1;
    weather_set_flag = 0;
    alarm_set_flag = 1;
    
    opacity_amount = 100;
}

void splash_off(void) {
    splash_flag = 0;
}

// TODO: A lot of the timekeeping functionality should be in its own timekeeper.c file

// TODO: should interrupts be disabled before incrementing this stuff?
void increment_seconds(void) {
    time++;
    if (MINUTES % 60 == 0) {
        new_minute_flag = 1;
    }
    if (time == alarm_time) {
        alarm_flag = 1;
    }
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

void play_alarm_sound(void) {
    alarm_flag = 1;
}

void stop_alarm_sound(void) {
    alarm_playing_flag = 0;
}

void call_ring_redraw(void) {
    draw_ring_flag = 1;
}

void call_grid_redraw(void) {
    draw_grid_flag = 1;
}

uint8_t redraw_ring_needed(void) {
    return draw_ring_flag;
}

uint8_t redraw_grid_needed(void) {
    return draw_grid_flag;
}

uint8_t alarm_is_playing(void) {
    return alarm_playing_flag;
}

uint8_t animation_is_playing(void) {
    return animation_flag;
}

uint8_t alarm_is_set(void) {
    return alarm_set_flag;
}

uint8_t weather_is_set(void) {
    return weather_set_flag;
}

// TODO: maybe put this in a timekeeper.c file? or maybe not?
uint8_t reached_new_minute(void) {
    return new_minute_flag;
}

uint8_t reached_alarm_time(void) {
    return alarm_flag;
}

void reset_ring_redraw(void) {
    draw_ring_flag = 0;
}

void reset_grid_redraw(void) {
    draw_grid_flag = 0;
}

void reset_redraw_flags(void) {
    reset_ring_redraw();
    reset_grid_redraw();
}

void reset_minute_flag(void) {
    new_minute_flag = 0;
}

void reset_alarm_flag(void) {
    alarm_flag = 0;
}

// sets the clock seconds, minutes, and hours counters to the supplied time in seconds.
void set_time(uint32_t new_time) {
    if (!(new_time >= 0L && new_time <= MAX_TIME)) {    // new_time (in seconds) isn't between
                                                        // 00:00:00 and 23:59:59 inclusive.
        return;
    }
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
        // Set each element of the led array to the combination of the rgb array and opacity array
        //update_pixel(&led_ring[i], (rgb_ring[i].r * opacity_ring[i]) / 100, 
        //    (rgb_ring[i].g * opacity_ring[i]) / 100, (rgb_ring[i].b * opacity_ring[i]) / 100);

        // This is using a constant opacity amount
        update_pixel(&led_ring[i], (rgb_ring[i].r * opacity_amount) / 100, 
            (rgb_ring[i].g * opacity_amount) / 100, (rgb_ring[i].b * opacity_amount) / 100);
    }

    // Adjust the grid 
    for (uint8_t i = 0; i < GRID_LEDS; i++) {
        // Set each element of the led array to the combination of the rgb array and opacity array
        //update_pixel(&led_grid[i], (rgb_grid[i].r * opacity_grid[i]) / 100, 
        //    (rgb_grid[i].g * opacity_grid[i]) / 100, (rgb_grid[i].b * opacity_grid[i]) / 100);

        // This is using a constant opacity amount
        update_pixel(&led_grid[i], (rgb_grid[i].r * opacity_amount) / 100, 
            (rgb_grid[i].g * opacity_amount) / 100, (rgb_grid[i].b * opacity_amount) / 100);
    }
}

void update_animation_frame(void) {
    // TODO animation logic
}

void update_display(void) {
    // Update entire ring
    if (draw_ring_flag) {
        for (uint8_t i = 0; RING_LEDS; i++) { 
            if (i * (60 / RING_LEDS) <= MINUTES) {
                update_pixel(&rgb_ring[i], RED);
            }
        }
    }

    // Add hour marker to grid if necessary
    if (hour_marker_flag) {
        update_pixel(&rgb_ring[HOURS % RING_LEDS], GREEN);
    }

    // Update grid
    if (draw_grid_flag) {
        if (animation_is_playing()) {      // display the animation
            // TODO animation logic
        } else {                        
            // Display the fours
            if (MINUTES % 5) {          // between 1-4 minutes past a 5 minute marker
                for (uint8_t i = 0; i < FOURS; i++) {
                    update_pixel(&rgb_grid[grid_minute_states[i]], RED);
                }
            } else {
                led_array_clear(rgb_grid, GRID_LEDS);
            }
        }
    }
}

// Setup the LED arrays, clearing the r, g, b values
static void init_leds(void) {
    // Clear the LED arrays
    led_array_clear(led_ring, RING_LEDS);
    led_array_clear(led_grid, GRID_LEDS);
    
    // Clear the RGB arrays
    led_array_clear(rgb_ring, RING_LEDS);
    led_array_clear(rgb_grid, GRID_LEDS);

    // Clear the opacity arrays
    opacity_array_clear(opacity_ring, RING_LEDS);
    opacity_array_clear(opacity_grid, GRID_LEDS);
}
