/*
 * project.c
 *
 * Written by Alex Subaric.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h> 

#include "clock.h"
#include "ir.h"
#include "ldr.h"
#include "ledarray.h"
#include "splash.h"
#include "timer.h"
#include "sound.h"

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <util/delay.h>

#define DISPLAY_UPDATE_DELAY         100    // 100 ms
#define DISPLAY_HOUR_MARKER_DELAY    500    // 500 ms 
#define DISPLAY_ANIMATION_TIME      4000    // 4 seconds
#define OPACITY_UPDATE_DELAY        2000    // 2 seconds
#define ANIMATION_FRAME_TIME         500    // 500 ms
#define PLAY_ALARM_TIME             3000	// 10 seconds

void initialise_hardware(void);
void initialise_clock(void);
void run_clock(void);
void update_clock(void);
void reset_clock(void);

/* The program's main function. */
int main(void) {
    initialise_hardware();
    //splash_screen();
    //splash_off();

    while (1) {
        initialise_clock();
        run_clock();
        //update_clock();
        //reset_clock();
    }
}

/* Initialises the hardware used (timers, ports, etc.) */
void initialise_hardware(void) {
    // Setup the main timer, providing an interrupt every millisecond
    init_timer0();

    //Setup timer1, used for sound
    init_timer1();

    // Turn on global interrupts
    sei();
}

/* Initialises the clock flags, timers, counters, and other variables. */
void initialise_clock(void) {
    init_clock();
    setup_sound();
}

/* Handles the main clock program (displaying time, animations, alarm, etc.) */
void run_clock(void) {
    // TODO only look at redrawing each pixel that is changed?
    uint32_t last_clock_tick_time;
    uint32_t last_display_time;
    uint32_t last_opacity_update_time;
    uint32_t start_animation_time;
    uint32_t start_alarm_time;

    last_clock_tick_time = last_display_time = last_opacity_update_time = start_animation_time = start_alarm_time = get_clock_ticks();

    while (1) {
        // Handle new second
        if (get_clock_ticks() - last_clock_tick_time >= 1000) {
            // One second has passed since the last clock tick, so increment the clock time by 1 second
            increment_seconds();
            call_ring_redraw();
			call_grid_redraw();
            last_clock_tick_time = get_clock_ticks();
        }

        // Turn on the weather animation
        if (weather_is_set() && reached_new_minute() && !animation_is_playing()) {
            play_weather_animation();
            start_animation_time = get_clock_ticks();
            reset_minute_flag();
        }

        // Update animation frame
        if (animation_is_playing() && (get_clock_ticks() - start_animation_time >= ANIMATION_FRAME_TIME)) {
            update_animation_frame();
            call_grid_redraw();
        }

        // If the weather animation is playing, turn it off after it's played for long enough
        if (animation_is_playing() && (get_clock_ticks() - start_animation_time >= DISPLAY_ANIMATION_TIME)) {
            stop_weather_animation();
            call_grid_redraw();
        }

        // Handle alarm
        if (alarm_is_set() && reached_alarm_time() && !alarm_is_playing()) {
            play_alarm_sound();
            // play_alarm();
            start_alarm_time = get_clock_ticks();
            reset_alarm_flag();
        }

        // If the alarm is playing, turn it off after it's played for long enough
        if (alarm_is_playing() && (get_clock_ticks() - start_alarm_time >= PLAY_ALARM_TIME)) {
            stop_alarm_sound();
        }

        // Handle brightness updating
        if (get_clock_ticks() - last_opacity_update_time >= OPACITY_UPDATE_DELAY) {
            update_opacity();
            last_opacity_update_time = get_clock_ticks();
        }

        // Toggle the hour marker
        if (get_clock_ticks() - last_display_time >= DISPLAY_HOUR_MARKER_DELAY) {
            toggle_hour_marker();
            call_ring_redraw();
        }

        // Handle new frame draw
        if ((redraw_ring_needed() || redraw_grid_needed()) 
                && (get_clock_ticks() - last_display_time >= DISPLAY_UPDATE_DELAY)) {
            // Time for a new 'frame' of the clock to be drawn & displayed
            update_display();
            // // Update the opacity for the ring and grid
            apply_opacity();
            // Frame has been drawn - reset the redraw flags
            reset_redraw_flags();

            last_display_time = get_clock_ticks();
        }

        // Update the current sound state
        update_sound();

        // Enable the LED chains
        show_display();
    }
}

/* Handles settings & config updates via IR communication. */
void update_clock(void) {
    // TODO this logic
}

/* The clock is gracefully stopped. */
void reset_clock(void) {
    // TODO this logic
    while (1) {     //hang the system
    }
}
