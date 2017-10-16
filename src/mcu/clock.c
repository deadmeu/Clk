/*
 * clock.c
 *
 * Written by Alex Subaric.
 */

#include <stdint.h>

#include "clock.h"
#include "unique_types.h"
#include "light_ws2812.h"
#include "ledarray.h"
#include "ldr.h"
#include "rtc.h"
#include "animation.h"
#include "pixel_colour.h"

#define CONV_TIME(h, m, s)      ((h*3600) + (m*60) + s)
#define HOURS(t)                (t / 3600)
#define MINUTES(t)              ((t - 3600 * HOURS(t)) / 60)
#define SECONDS(t)              ((t - 3600 * HOURS(t)) - 60 * MINUTES(t))
#define FOURS                   (MINUTES(time) % 5)
#define NEW_CYCLE_TIME          (time == 0L || time == MID_DAY_TIME)

#define HOUR_MARKER_COLOUR  GREEN
//#define ANTE_MERIDIEM       0
// #define RING_PIN            0
//#define POST_MERIDIEM       1
// #define GRID_PIN            1

static void init_leds(void);
static void set_default_values(void);

// LED arrays
struct cRGB led_ring[RING_LEDS];
struct cRGB led_grid[GRID_LEDS];

// RGB arrays
struct cRGB rgb_ring[RING_LEDS];
struct cRGB rgb_grid[GRID_LEDS];

                                            // [0]   [3] ~Index numbers~
                                            // {1 0 0 2} Grid is arranged in a 
                                            // {0 0 0 0} 4x4 layout with minute 
uint8_t grid_minute_states[4] = { 0,  3,    // {0 0 0 0} markers in each corner.
                                 12, 15};   // {3 0 0 4} 
                                            // [12] [15] ~Index numbers~

// Meridiem colours
pcol_t meridiem_colours[2];

// Weather array
wtype_t current_weather[2];

// Other variables
static volatile uint32_t time;
static volatile uint32_t alarm_time;
static volatile uint8_t opacity;
static volatile uint8_t total_animation_loops;
static volatile uint8_t playing_weather_index;

static volatile wtype_t playing_weather;

// TODO maybe use a bitfield for these?
// Flags
static uint8_t new_reset_flag;
static uint8_t splash_flag;
static uint8_t animation_flag;
static uint8_t alarm_flag;
static uint8_t hour_marker_flag;
static uint8_t new_minute_flag;
static uint8_t time_changed_flag;
static uint8_t alarm_playing_flag;
static uint8_t weather_set_flag;
static uint8_t eeprom_set_flag;
static uint8_t alarm_set_flag;
static uint8_t draw_ring_flag;
static uint8_t draw_grid_flag;
static uint8_t meridiem_flag;
static uint8_t usart_enabled_flag;

void init_clock(void) {
    init_leds();
    
    stop_alarm_sound();
    stop_weather_animation();

    reset_alarm_flag();
    reset_minute_flag();

    enable_usart();
    reset_time_changed_flag();

    call_ring_redraw();
    call_grid_redraw();

    set_meridiem_colours(ANTE, RED);
    set_meridiem_colours(POST, BLUE);

    // weather animation frame reset
    reset_frame_count();

    if (is_new_reset() || !eeprom_is_set()) {
        set_default_values();
    }

    update_meridiem();

    hour_marker_flag = 1;
    splash_flag = 1;
    total_animation_loops = 0;

    opacity = 30;
}

static void set_default_values(void) {
    time = 0;
    alarm_time = 0;
    alarm_set_flag = 0;
    weather_set_flag = 0;
    eeprom_set_flag = 0;
}

void splash_off(void) {
    splash_flag = 0;
}

void enable_new_reset(void) {
    new_reset_flag = 1;
}

void disable_new_reset(void) {
    new_reset_flag = 0;
}

// TODO: A lot of the timekeeping functionality should be in its own timekeeper.c file

// TODO: should interrupts be disabled before incrementing this stuff?
void increment_seconds(void) {
    // if (time++ == MAX_TIME) {
    //     set_time(0L);
    // }
    time = (time + 1) % (MAX_TIME + 1);
    
    update_new_minute_flag();

    if (time == alarm_time) {
        alarm_flag = 1;
    }
    if (NEW_CYCLE_TIME) {
        update_meridiem();
    }
}

void update_new_minute_flag(void) {
    if (SECONDS(time) % 60 == 0) {
        new_minute_flag = 1;
    }
}

uint8_t clock_update_time(void) {
    if (!rtc_running()) return 0;

    uint32_t new_time = rtc_get_time();
    
    time_changed_flag = new_time != time;
    if (clock_time_changed()) update_new_minute_flag();
    
    if (!set_time(new_time)) return 0;
    
    return 1;
}

uint8_t clock_time_changed(void) {
    return time_changed_flag;
}

void reset_time_changed_flag(void) {
    time_changed_flag = 0;
}

void set_meridiem_colours(merid_t merid, pcol_t col) {
    if (merid != ANTE && merid != POST) {
        return;
    }
    meridiem_colours[merid] = col;
}

void update_meridiem(void) {
    meridiem_flag = (time < MID_DAY_TIME) ? ANTE : POST;
}

void play_weather_animation(void) {
    animation_flag = 1;
    playing_weather_index = 0;
}

void update_playing_weather(void) {
    playing_weather = current_weather[playing_weather_index];
}

void stop_weather_animation(void) {
    animation_flag = 0;
}

void play_alarm_sound(void) {
    alarm_playing_flag = 1;
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

uint8_t is_new_reset(void) {
    return new_reset_flag;
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

uint8_t eeprom_is_set(void) {
    return eeprom_set_flag;
}

uint8_t weather_is_set(void) {
    return weather_set_flag;
}

uint8_t usart_enabled(void) {
    return usart_enabled_flag;
}

// TODO: maybe put this in a timekeeper.c file? or maybe not?
uint8_t reached_new_minute(void) {
    return new_minute_flag;
}

uint8_t reached_alarm_time(void) {
    return alarm_flag;
}

uint8_t get_alarm_seconds(void) {
    return SECONDS(alarm_time);
}

uint8_t get_alarm_minutes(void) {
    return MINUTES(alarm_time);
}

uint8_t get_alarm_hours(void) {
    return HOURS(alarm_time);
}

uint8_t get_clock_seconds(void) {
    return SECONDS(time);
}

uint8_t get_clock_minutes(void) {
    return MINUTES(time);
}

uint8_t get_clock_hours(void) {
    return HOURS(time);
}

uint32_t get_time(void) {
    return time;
}

void disable_alarm(void) {
    alarm_set_flag = 0;
}

void enable_alarm(void) {
    alarm_set_flag = 1;
}

void enable_usart(void) {
    usart_enabled_flag = 1;
}

void disable_usart(void) {
    usart_enabled_flag = 0;
}

void disable_weather(void) {
    weather_set_flag = 0;
}

void enable_weather(void) {
    weather_set_flag = 1;
}

void disable_eeprom(void) {
    eeprom_set_flag = 0;
}

void enable_eeprom(void) {
    eeprom_set_flag = 1;
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

// sets the clock seconds, minutes, and hours counters to the supplied time 
// in seconds.
uint8_t set_time(uint32_t new_time) {
    if (!(new_time >= 0L && new_time <= MAX_TIME)) { // new_time (in seconds) 
                                                     // isn't between 00:00:00 
                                                     // and 23:59:59 inclusive.
        return 0;
    }
    time = new_time;
    return 1;
}

// TODO change these magic numbers to MAX_HOUR, MIN_SECOND, SECONDS_IN_HOUR, etc.
uint8_t set_split_clock_time(uint8_t h, uint8_t m, uint8_t s) {
    if (!(h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60)) {
        return 0;
    }
    time = CONV_TIME(h, m, s);
    return 1;
}

uint8_t set_alarm_time(uint32_t new_time) {
    if (!(new_time >= 0L && new_time <= MAX_TIME)) { // new_time (in seconds) 
                                                     // isn't between 00:00:00 
                                                     // and 23:59:59 inclusive.
        return 0;
    }
    alarm_time = new_time;
    return 1;
}

// TODO change these magic numbers to MAX_HOUR, MIN_SECOND, SECONDS_IN_HOUR, etc.
uint8_t set_split_alarm_time(uint8_t h, uint8_t m, uint8_t s) {
    if (!(h >= 0 && h < 24 && m >= 0 && m < 60 && s >= 0 && s < 60)) {
        return 0;
    }
    alarm_time = CONV_TIME(h, m, s);
    return 1;
}

uint8_t set_opacity(uint8_t new_opacity) {
    if (!(new_opacity >= MIN_OPACITY && new_opacity <= MAX_OPACITY)) {
        return 0;
    }
    opacity = new_opacity;
    return 1;    
}

uint8_t set_weather(wtype_t weather_one, wtype_t weather_two) {
    if (!(weather_one >= SUNNY && weather_one <= WINDY 
            && weather_two >= SUNNY && weather_two <= WINDY)) {
        return 0;
    }
    current_weather[0] = weather_one;
    current_weather[1] = weather_two;
    weather_set_flag = 1;
    return 1;
}

wtype_t get_weather_one(void) {
    return (weather_is_set()) ? current_weather[0] : SUNNY;
}

wtype_t get_weather_two(void) {
    return (weather_is_set()) ? current_weather[1] : SUNNY;
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
        // Set each element of the led array to the combination of the rgb array
        // and opacity value.
        update_pixel_rgb(&led_ring[i], (rgb_ring[i].r * opacity) / 100, 
            (rgb_ring[i].g * opacity) / 100, (rgb_ring[i].b * opacity) / 100);
    }

    // Adjust the grid 
    for (uint8_t i = 0; i < GRID_LEDS; i++) {
        // Set each element of the led array to the combination of the rgb array
        // and opacity value.
        update_pixel_rgb(&led_grid[i], (rgb_grid[i].r * opacity) / 100, 
            (rgb_grid[i].g * opacity) / 100, (rgb_grid[i].b * opacity) / 100);
    }
}

void update_animation_frame(void) {
    if (incr_frame_count() == ANIMATION_FRAMES) {
        total_animation_loops++;
    }

    if (total_animation_loops == ANIMATION_LOOPS) {
        playing_weather_index++;
        update_playing_weather();
    }
}

void update_opacity(void) {
    set_opacity(get_ldr_opacity());
}

void update_display(void) {
    // Update entire ring
    if (draw_ring_flag) {
        for (uint8_t i = 0; i < RING_LEDS; i++) { 
            if (i * (60 / RING_LEDS) <= MINUTES(time)) {
                update_pixel_col(&rgb_ring[i], meridiem_colours[meridiem_flag]);
            } else {
                update_pixel_col(&rgb_ring[i], BLACK);
            }
        }
    }

    // Add hour marker to grid if necessary
    if (hour_marker_flag) {
        update_pixel_col(&rgb_ring[HOURS(time) % RING_LEDS], HOUR_MARKER_COLOUR);
    }
	// TODO change grid coour to meridiem
    // Update grid
    if (draw_grid_flag) {
        if (animation_is_playing()) {      // display the animation
            for (uint8_t i = 0; i < GRID_LEDS; i++) {
                update_pixel_col(&rgb_grid[i], get_frame_pixel(playing_weather, i));
            }
        // Display the fours
        } else if (MINUTES(time) % 5) {    // between 1-4 minutes past a 5 minute marker
            led_array_clear(rgb_grid, GRID_LEDS);
            for (uint8_t i = 0; i < FOURS; i++) {
                update_pixel_col(&rgb_grid[grid_minute_states[i]], 
                                    meridiem_colours[meridiem_flag]);
            }
        // Clear the array
        } else {
            led_array_clear(rgb_grid, GRID_LEDS);
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
}
