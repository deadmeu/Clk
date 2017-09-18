/*
 * clock.c
 *
 * Written by Alex Subaric.
 */

#include <stdint.h>

#include "clock.h"
#include "ledarray.h"
#include "ldr.h"
#include "pixel_colour.h"

#define RING_LEDS           12
#define GRID_LEDS           16
#define MAX_TIME            86399L              // 23:59:59 converted to seconds. L for long (32 bit) type
#define MID_DAY_TIME        43200L              // 12:00:00 (midday) converted to seconds 

#define HOURS               (time / 3600)
#define MINUTES             ((time - 3600 * HOURS) / 60)
#define SECONDS             ((time - 3600 * HOURS) - 60 * MINUTES)
#define FOURS               (MINUTES % 5)
#define NEW_CYCLE_TIME      (time == 0L || time == MID_DAY_TIME)

#define HOUR_MARKER_COLOUR  GREEN
#define ANTE_MERIDIEM       0
#define RING_PIN            0
#define POST_MERIDIEM       1
#define GRID_PIN            1

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
uint8_t grid_minute_states[4] = {12,  13,     // {0 0 0 0} in each corner.
                                14, 15};    // {3 0 0 4}
                                            // [12] [15] ~Index numbers~

// Meridiem colours
uint8_t meridiem_colours[2][3];

// Other variables
uint32_t time;
uint32_t alarm_time;
uint8_t opacity_amount;

// Flags
static uint8_t splash_flag;
static uint8_t animation_flag;
static uint8_t alarm_flag;
static uint8_t hour_marker_flag;
static uint8_t new_minute_flag;
static uint8_t alarm_playing_flag;
static uint8_t weather_set_flag;
static uint8_t alarm_set_flag;
static uint8_t draw_ring_flag;
static uint8_t draw_grid_flag;
static uint8_t meridiem_flag;

void init_clock(void) {
    init_leds();
    
    stop_alarm_sound();
    stop_weather_animation();

    reset_alarm_flag();
    reset_minute_flag();

    call_ring_redraw();
    call_grid_redraw();

    set_meridiem_colours(ANTE_MERIDIEM, RED);
    set_meridiem_colours(POST_MERIDIEM, BLUE);
  
    time = MAX_TIME-4;       // 
    alarm_time = 0;    // 1 minute past midnight
    splash_flag = 1;
    hour_marker_flag = 1;
    weather_set_flag = 0;
    alarm_set_flag = 1;
    opacity_amount = 30;

    update_meridiem();
}

void splash_off(void) {
    splash_flag = 0;
}

// TODO: A lot of the timekeeping functionality should be in its own timekeeper.c file

// TODO: should interrupts be disabled before incrementing this stuff?
void increment_seconds(void) {
    // if (time++ == MAX_TIME) {
    //     set_time(0L);
    // }
    time = (time + 1) % (MAX_TIME + 1);
    if (MINUTES % 60 == 0) {
        new_minute_flag = 1;
    }
    if (time == alarm_time) {
        alarm_flag = 1;
    }
    if (NEW_CYCLE_TIME) {
        update_meridiem();
    }
}

void set_meridiem_colours(uint8_t meridiem, uint8_t r, uint8_t g, uint8_t b) {
    if (!(meridiem == ANTE_MERIDIEM || meridiem == POST_MERIDIEM)) {
        return;
    }
    if (!(r >= 0 && g >= 0 && b >= 0 && r <= 255 && g <= 255 && b <= 255)) {
        return;
    }

    meridiem_colours[meridiem][0] = r;
    meridiem_colours[meridiem][1] = g;
    meridiem_colours[meridiem][2] = b;
}

void update_meridiem(void) {
    meridiem_flag = (time < MID_DAY_TIME) ? ANTE_MERIDIEM : POST_MERIDIEM;
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
uint8_t set_time(uint32_t new_time) {
    if (!(new_time >= 0L && new_time <= MAX_TIME)) {    // new_time (in seconds) isn't between
                                                        // 00:00:00 and 23:59:59 inclusive.
        return 0;
    }
    time = new_time;
    return 1;
}

uint8_t set_alarm_time(uint32_t new_time) {
    if (!(new_time >= 0L && new_time <= MAX_TIME)) {    // new_time (in seconds) isn't between
                                                        // 00:00:00 and 23:59:59 inclusive.
        return 0;
    }
    alarm_time = new_time;
    return 1;
}

uint8_t set_weather(uint8_t weather1_type, uint8_t weather2_type) {
    return 0;
}

void show_display(void) {
    enable_leds(led_ring, RING_LEDS, RING_PIN);
    enable_leds(led_grid, GRID_LEDS, GRID_PIN);
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

void update_opacity(void) {
    opacity_amount = get_ldr_opacity();
}

void update_display(void) {
    // Update entire ring
    if (draw_ring_flag) {
        for (uint8_t i = 0; i < RING_LEDS; i++) { 
            if (i * (60 / RING_LEDS) <= MINUTES) {
                update_pixel(&rgb_ring[i], meridiem_colours[meridiem_flag][0], 
                    meridiem_colours[meridiem_flag][1], 
                    meridiem_colours[meridiem_flag][2]);
            } else {
                update_pixel(&rgb_ring[i], BLACK);
            }
        }
    }

    // Add hour marker to grid if necessary
    if (hour_marker_flag) {
        update_pixel(&rgb_ring[HOURS % RING_LEDS], HOUR_MARKER_COLOUR);
    }
	// TODO change grid coour to meridiem
    // Update grid
    if (draw_grid_flag) {
        if (animation_is_playing()) {      // display the animation
            // TODO animation logic
        } else {                        
            // Display the fours
            if (MINUTES % 5) {          // between 1-4 minutes past a 5 minute marker
                for (uint8_t i = 0; i < FOURS; i++) {
                    update_pixel(&rgb_grid[grid_minute_states[i]], meridiem_colours[meridiem_flag][0], meridiem_colours[meridiem_flag][1], meridiem_colours[meridiem_flag][2]);
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
