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


#define HOURS(t)                (t / 3600)
#define MINUTES(t)              ((t - 3600 * HOURS(t)) / 60)
#define SECONDS(t)              ((t - 3600 * HOURS(t)) - 60 * MINUTES(t))
#define FOURS                   (MINUTES(time) % 5)
#define NEW_CYCLE_TIME          (time == 0L || time == MID_DAY_TIME)
#define FOURS_LEDS              2

#define HOUR_MARKER_COLOUR  GREEN

static void init_leds(void);
static void set_default_values(void);

// LED arrays
struct cRGB led_ring[RING_LEDS];
struct cRGB led_grid[GRID_LEDS];

// RGB arrays
struct cRGB rgb_ring[RING_LEDS];
struct cRGB rgb_grid[GRID_LEDS];




uint8_t grid_minute_states[4][FOURS_LEDS] = { {2 ,7}, {11,14},
                                              {13,8}, { 4,1 } };


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

// Flags
static uint8_t new_reset_flag;
static uint8_t animation_flag;
static uint8_t ir_animation_flag;
static uint8_t alarm_animation_flag;
static uint8_t splash_animation_flag;
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

// Initialises clock variables
void init_clock(void) {
    init_leds();
    
    stop_alarm_sound();
    stop_animation();

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

	if (is_new_reset()) {
		set_default_values();
	}

    update_meridiem();

    hour_marker_flag = 1;
    total_animation_loops = 0;
    ir_animation_flag = 0;
    alarm_animation_flag = 0;
    alarm_playing_flag = 0;

    opacity = 30;
}

// Default values to be applied on a hard reset
static void set_default_values(void) {
    time = MAX_TIME;
    alarm_time = 0;
    alarm_set_flag = 0;
    weather_set_flag = 0;
    eeprom_set_flag = 1;
	play_splash_animation();
}

// Enable a hard reset
void enable_new_reset(void) {
    new_reset_flag = 1;
}

// Disables a hard reset
void disable_new_reset(void) {
    new_reset_flag = 0;
}

// Increments a second on the clock
void increment_seconds(void) {
    time = (time + 1) % (MAX_TIME + 1);
    
    update_new_minute_flag();

    if (time == alarm_time) {
        alarm_flag = 1;
    }
    if (NEW_CYCLE_TIME) {
        update_meridiem();
    }
}

// Toggle when a new minute is set
void update_new_minute_flag(void) {
    if (SECONDS(time) % 60 == 0) {
        new_minute_flag = 1;
    }
}

// Update the clock time to the rtc time
uint8_t clock_update_time(void) {
    if (!rtc_running()) return 0;

    uint32_t new_time = rtc_get_time();
    
    time_changed_flag = new_time != time;
    if (clock_time_changed()) update_new_minute_flag();
    
    if (!set_time(new_time)) return 0;
	
	update_meridiem();
    
    return 1;
}

// Returns true if the previously checked time is different to the current time
uint8_t clock_time_changed(void) {
    return time_changed_flag;
}

// Clears the reset time flag
void reset_time_changed_flag(void) {
    time_changed_flag = 0;
}

// Set the colours for AM/PM
void set_meridiem_colours(merid_t merid, pcol_t col) {
    if (merid != ANTE && merid != POST) {
        return;
    }
    meridiem_colours[merid] = col;
}

// Updates the AM/PM meridiem
void update_meridiem(void) {
    meridiem_flag = (time < MID_DAY_TIME) ? ANTE : POST;
}

// Enable the playing of the IR animation
void play_ir_animation(void) {
    play_animation();
    ir_animation_flag = 1;
}

// Enable the playing of the splash animation
void play_splash_animation(void) {
    play_animation();
    splash_animation_flag = 1;
}

// Returns true if the splash animation is playing
uint8_t splash_playing(void) {
	return splash_animation_flag;
}

// Enable the playing of the alarm animation
void play_alarm_animation(void) {
    play_animation();
    alarm_animation_flag = 1;
}

// Enable the playing of the weather animation
void play_weather_animation(void) {
    play_animation();
    playing_weather_index = 0;
    total_animation_loops = 0;
    update_playing_weather();
}

// Enable animations
void play_animation(void) {
    animation_flag = 1;
    reset_frame_count();
}

// Updates the currently playing weather
void update_playing_weather(void) {
    playing_weather = current_weather[playing_weather_index];
}

// Disables all animations
void stop_animation(void) {
    animation_flag = 0;
    ir_animation_flag = 0;
    alarm_animation_flag = 0;
    splash_animation_flag = 0;
}

// Plays the alarm sound
void play_alarm_sound(void) {
    alarm_playing_flag = 1;
}

// Stops the alarm sound
void stop_alarm_sound(void) {
    alarm_playing_flag = 0;
}

// Queues the redrawing of the ring LEDs
void call_ring_redraw(void) {
    draw_ring_flag = 1;
}

// Queues the redrawing of the grid LEDs
void call_grid_redraw(void) {
    draw_grid_flag = 1;
}

// Returns true if a ring redraw should take place
uint8_t redraw_ring_needed(void) {
    return draw_ring_flag;
}

// Returns true if a grid redraw should take place
uint8_t redraw_grid_needed(void) {
    return draw_grid_flag;
}

// Returns the hard reset flag
uint8_t is_new_reset(void) {
    return new_reset_flag;
}

// Returns true if alarm is playing
uint8_t alarm_is_playing(void) {
    return alarm_playing_flag;
}

// Returns true if animation is playing
uint8_t animation_is_playing(void) {
    return animation_flag;
}

// Returns true if an alarm is set
uint8_t alarm_is_set(void) {
    return alarm_set_flag;
}

// Returns true if eeprom is enabled
uint8_t eeprom_is_set(void) {
    return eeprom_set_flag;
}

// Returns true if the weather is set
uint8_t weather_is_set(void) {
    return weather_set_flag;
}

// Returns true if usart is enabled
uint8_t usart_enabled(void) {
    return usart_enabled_flag;
}

// Returns true if a new minute has been reached
uint8_t reached_new_minute(void) {
    return new_minute_flag;
}

// Returns true if the alarm time has been reached
uint8_t reached_alarm_time(void) {
    return alarm_flag;
}

// Returns the seconds value of the alarm time
uint8_t get_alarm_seconds(void) {
    return SECONDS(alarm_time);
}

// Returns the minutes value of the alarm time
uint8_t get_alarm_minutes(void) {
    return MINUTES(alarm_time);
}

// Returns the hours value of the alarm time
uint8_t get_alarm_hours(void) {
    return HOURS(alarm_time);
}

// Returns the seconds value of the clock time
uint8_t get_clock_seconds(void) {
    return SECONDS(time);
}

// Returns the minutes value of the clock time
uint8_t get_clock_minutes(void) {
    return MINUTES(time);
}

// Returns the hours value of the clock time
uint8_t get_clock_hours(void) {
    return HOURS(time);
}

// Returns the clock time (in seconds)
uint32_t get_time(void) {
    return time;
}

// Disables the alarm set flag
void disable_alarm(void) {
    alarm_set_flag = 0;
}

// Enables the alarm set flag
void enable_alarm(void) {
    alarm_set_flag = 1;
}

// Enables the usart flag
void enable_usart(void) {
    usart_enabled_flag = 1;
}

// Disables the usart flag
void disable_usart(void) {
    usart_enabled_flag = 0;
}

// Disables the weather set flag
void disable_weather(void) {
    weather_set_flag = 0;
}

// Enables the weather set flag
void enable_weather(void) {
    weather_set_flag = 1;
}

// Disables the eeprom flag
void disable_eeprom(void) {
    eeprom_set_flag = 0;
}

// Enables the eeprom
void enable_eeprom(void) {
    eeprom_set_flag = 1;
}

// Clears the ring redraw flag
void reset_ring_redraw(void) {
    draw_ring_flag = 0;
}

// Clears the grid redraw flag
void reset_grid_redraw(void) {
    draw_grid_flag = 0;
}

// Clears both grid and ring redraw flags
void reset_redraw_flags(void) {
    reset_ring_redraw();
    reset_grid_redraw();
}

// Clears the new minute flag
void reset_minute_flag(void) {
    new_minute_flag = 0;
}

// Clears the alarm flag
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

// Sets the clock time using the supplied hour,min,second arguments
uint8_t set_split_clock_time(uint8_t h, uint8_t m, uint8_t s) {
    if (!(h >= MIN_HOUR && h <= MAX_HOUR && m >= MIN_MINUTE && m <= MAX_MINUTE 
            && s >= MIN_SECOND && s < MAX_SECOND)) {
        return 0;
    }
    time = CONV_TIME(h, m, s);
	update_meridiem();
    return 1;
}

// Sets the alarm time to the supplied time in seconds
uint8_t set_alarm_time(uint32_t new_time) {
    if (!(new_time >= 0L && new_time <= MAX_TIME)) { // new_time (in seconds) 
                                                     // isn't between 00:00:00 
                                                     // and 23:59:59 inclusive.
        return 0;
    }
    alarm_time = new_time;
    return 1;
}

// Sets the alarm time using the supplied hour,min,second arguments
uint8_t set_split_alarm_time(uint8_t h, uint8_t m, uint8_t s) {
    if (!(h >= MIN_HOUR && h <= MAX_HOUR && m >= MIN_MINUTE && m <= MAX_MINUTE 
            && s >= MIN_SECOND && s < MAX_SECOND)) {
        return 0;
    }
    alarm_time = CONV_TIME(h, m, s);
    return 1;
}

// Updates the opacity with the supplied opacity value
uint8_t set_opacity(uint8_t new_opacity) {
    if (!(new_opacity >= MIN_OPACITY && new_opacity <= MAX_OPACITY)) {
        return 0;
    }
    opacity = new_opacity;
    return 1;    
}

// Sets the weather to the supplied weather types
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

// Returns the first weather type
wtype_t get_weather_one(void) {
    return (weather_is_set()) ? current_weather[0] : SUNNY;
}

// Returns the second weather type
wtype_t get_weather_two(void) {
    return (weather_is_set()) ? current_weather[1] : SUNNY;
}

// Enables the grid and ring LEDs
void show_display(void) {
    enable_leds(led_ring, RING_LEDS, RING_PIN);
    enable_leds(led_grid, GRID_LEDS, GRID_PIN);
}

// Toggles the drawing of the hour marker
void toggle_hour_marker(void) {
    hour_marker_flag ^= 1;
}

// Applies the opacity with the rgb array, to the led array
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

// Moves to the next animation frame
void update_animation_frame(void) {
    if (ir_animation_flag || alarm_animation_flag) {
		incr_frame_count();
		return;
    }

    if (total_animation_loops == ANIMATION_LOOPS) {
        total_animation_loops = 0;
        playing_weather_index = (playing_weather_index + 1) % ANIMATION_FRAMES;
        update_playing_weather();
    }

    if (incr_frame_count() == ANIMATION_FRAMES-1) {
        total_animation_loops++;
    }

    
}

// Updates the opacity with the LDR value
void update_opacity(void) {
    set_opacity(get_ldr_opacity());
}

// Updates the display's RGB array
void update_display(void) {
    // Update entire ring
    if (draw_ring_flag) {
        for (uint8_t i = 0; i < RING_LEDS; i++) { 
            if (i * (60 / RING_LEDS) <= MINUTES(time) && !ir_animation_flag && !splash_animation_flag) {
                update_pixel_col(&rgb_ring[i], meridiem_colours[meridiem_flag]);
            } else {
                update_pixel_col(&rgb_ring[i], BLACK);
            }
        }
    }

    // Add hour marker to grid if necessary
    if (hour_marker_flag && !ir_animation_flag && !splash_animation_flag) {
        update_pixel_col(&rgb_ring[HOURS(time) % RING_LEDS], HOUR_MARKER_COLOUR);
    }

    // Update grid
    if (draw_grid_flag) {
        if (animation_is_playing()) {      // display the animation
            if (splash_animation_flag) {
                for (uint8_t i = 0; i < GRID_LEDS; i++) {
                    update_pixel_col(&rgb_grid[i], get_splash_frame_pixel(i));
                }
            } else if (ir_animation_flag) {
                for (uint8_t i = 0; i < GRID_LEDS; i++) {
                    update_pixel_col(&rgb_grid[i], get_ir_frame_pixel(i));
                }
            } else if (alarm_animation_flag) {
                for (uint8_t i = 0; i < GRID_LEDS; i++) {
                    update_pixel_col(&rgb_grid[i], get_alarm_frame_pixel(i));
                }
            } else {
                for (uint8_t i = 0; i < GRID_LEDS; i++) {
                    update_pixel_col(&rgb_grid[i], get_frame_pixel(playing_weather, i));
                }
            }
        // Display the fours
        } else if (MINUTES(time) % 5) {    // between 1-4 minutes past a 5 minute marker
            led_array_clear(rgb_grid, GRID_LEDS);
            for (uint8_t i = 0; i < FOURS; i++) {
                for (uint8_t j = 0; j < FOURS_LEDS; j++) {
                    update_pixel_col(&rgb_grid[grid_minute_states[i][j]], 
                                        meridiem_colours[meridiem_flag]);
                    }
            }
        // Clear the array
        } else {
            led_array_clear(rgb_grid, GRID_LEDS);
        }
    }
}

// Immediately clears the display
void clear_display(void) {
    init_leds();
    show_display();
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
