/*
 * project.c
 *
 * Written by Alex Subaric.
 * The license conditions and warranties of this software can be found at
 * http://www.nongnu.org/avr-libc/LICENSE.txt
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h> 

#include "clock.h"
#include "ir.h"
#include "ldr.h"
#include "rtc.h"
#include "ledarray.h"
#include "timer.h"
#include "sound.h"
#include "progmem.h"

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <util/delay.h>

#define DISPLAY_UPDATE_DELAY         100    // 100 ms
#define DISPLAY_HOUR_MARKER_DELAY    500    // 500 ms 
#define DISPLAY_ANIMATION_TIME      4000    // 4 seconds
#define RTC_UPDATE_TIME            30000    // 30 seconds
#define OPACITY_UPDATE_DELAY        1000    // 1 s
#define ANIMATION_FRAME_TIME         500    // 500 ms
#define PLAY_ALARM_TIME             10000	// 10 seconds
#define MAX_CLOCK_UPDATE_TIME       3000    // 3 seconds
#define SECOND_INCREMENT_TIME       1000    // 1 second
#define IR_LED_PLAYTIME             1500    // 1 second
#define IR_LED_DELAY                250     // 250 ms
// #define SPLASH_PLAY_TIME            4000    // 4 seconds

void initialise_hardware(void);
void initialise_clock(void);
void run_clock(void);
void update_clock(void);
void reset_clock(void);

/* The program's main function. */
int main(void) {
    initialise_hardware();
    while (1) {
        initialise_clock();
        run_clock();
        update_clock();
        reset_clock();
    }

    return 0;
}

/* Initialises the hardware used (timers, ports, etc.) */
void initialise_hardware(void) {
    // Tell the clock a hard reset has occurred.
    enable_new_reset();

    // Setup the main timer, providing an interrupt every millisecond
    init_timer0();

    //Setup timer1, used for sound
    init_timer1();

    // Setup ADC for brightness checking from the LDR
    init_ldr();

    // Setup RTC for timekeeping
    init_rtc();

    if (!rtc_started()) {
       // Error with starting the RTC.
    }

    // Setup UART and IR data buffers
    init_ir();

    // Turn on global interrupts
    sei();
}

/* Initialises the clock flags, timers, counters, and other variables. */
void initialise_clock(void) { 
    init_clock();

    setup_sound();

    eeprom_read_data();
    eeprom_set_data();
    enable_eeprom();

    clock_update_time();

    enable_usart();
}

/* Handles the main clock program (displaying time, animations, alarm, etc.) */
void run_clock(void) {
    /* Variables used for tracking time-delayed events. */
    uint32_t start_splash_time;
    uint32_t last_clock_tick_time;
    uint32_t last_display_time;
    uint32_t last_hour_marker_display_time;
    uint32_t last_opacity_update_time;
    uint32_t last_rtc_update_time;
    uint32_t start_animation_time;
    uint32_t last_animation_frame_time;
    uint32_t start_alarm_time;

    /* Set the time-tracking variables to the current time. */
    last_clock_tick_time = last_display_time = last_hour_marker_display_time 
            = last_opacity_update_time = start_animation_time = start_alarm_time
            = last_rtc_update_time =  last_animation_frame_time = start_splash_time
            = get_clock_ticks();

    while (1) {
        if (clock_is_updating()) {
            if (!connection_established()) {
                // Clock is receiving IR transmission, move to the next clock state.
                break;
            }
            play_splash_animation();
            reset_updating_flag();
            reset_connection_flag();
            break;
        }

        // Handle new second
        if (get_clock_ticks() - last_clock_tick_time >= SECOND_INCREMENT_TIME) {
            // One second has passed since the last clock tick, so increment 
            // the clock time by 1 second.
            if (!clock_time_changed()) {
                increment_seconds();
            }
            call_ring_redraw();
            call_grid_redraw();
            reset_time_changed_flag();
            last_clock_tick_time = get_clock_ticks();
        }

        // Handle a remote time update from the RTC.
        if (get_clock_ticks() - last_rtc_update_time >= RTC_UPDATE_TIME) {
           clock_update_time();
           last_rtc_update_time = get_clock_ticks();
        }

        // Turn on the weather animation
        if (weather_is_set() && reached_new_minute() 
                             && !animation_is_playing()) {
            reset_minute_flag();
            play_weather_animation();
            call_grid_redraw();
            start_animation_time = get_clock_ticks();
            last_animation_frame_time = start_animation_time;
        }

        // Update animation frame
        if (animation_is_playing() && (get_clock_ticks() - last_animation_frame_time 
                                            >= ANIMATION_FRAME_TIME)) {
            update_animation_frame();
            call_grid_redraw();
            last_animation_frame_time = get_clock_ticks();
        }

        // If an animation is playing, turn it off after it's played 
        // for long enough (4 seconds).
        if (animation_is_playing() && !alarm_is_playing() 
                && (get_clock_ticks() - start_animation_time >= DISPLAY_ANIMATION_TIME)) {
            stop_animation();
            call_grid_redraw();
        }

        // If an animation is playing, turn it off after it's played 
        // for long enough (4 seconds).
        if (animation_is_playing() && alarm_is_playing() 
                && (get_clock_ticks() - start_animation_time >= PLAY_ALARM_TIME)) {
            stop_animation();
            call_grid_redraw();
        }

        // Handle alarm
        if (alarm_is_set() && reached_alarm_time() && !alarm_is_playing()) {
            play_alarm_sound();
            play_alarm_animation();
            start_alarm_time = get_clock_ticks();
            start_animation_time = get_clock_ticks();
            reset_alarm_flag();
        }

        // If the alarm is playing, turn it off after it's played for 
        // long enough.
        if (alarm_is_playing() && (get_clock_ticks() - start_alarm_time 
                                        >= PLAY_ALARM_TIME)) {
            stop_alarm_sound();
            stop_animation();
        }

        // Handle brightness updating
        if (get_clock_ticks() - last_opacity_update_time 
                >= OPACITY_UPDATE_DELAY) {
            update_opacity();
            call_ring_redraw();
            call_grid_redraw();
            last_opacity_update_time = get_clock_ticks();
        }

        // Toggle the hour marker
        if (get_clock_ticks() - last_hour_marker_display_time 
                >= DISPLAY_HOUR_MARKER_DELAY) {
            toggle_hour_marker();
            call_ring_redraw();
            last_hour_marker_display_time = get_clock_ticks();
        }

        // Handle new frame draw
        if ((redraw_ring_needed() || redraw_grid_needed()) 
                && (get_clock_ticks() - last_display_time 
                        >= DISPLAY_UPDATE_DELAY)) {
            // Time for a new 'frame' of the clock to be drawn & displayed
            update_display();
            // Apply the opacity for the ring and grid
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
    uint32_t start_update_time = get_clock_ticks();
    uint32_t last_animation_frame_time = start_update_time;

    clear_display();

    play_ir_animation();
    call_grid_redraw();

    while (usart_enabled() || (get_clock_ticks() - start_update_time < IR_LED_PLAYTIME)) {
        if (get_clock_ticks() - start_update_time >= MAX_CLOCK_UPDATE_TIME) {
            // ERROR IN UART COMMUNICATION
            disable_usart();
            stop_animation();
            clear_display();

            // Cancel this read
            return;
        }

        // Display the IR animation
        if (get_clock_ticks() - last_animation_frame_time >= IR_LED_DELAY) {
            update_animation_frame();
            call_grid_redraw();
            last_animation_frame_time = get_clock_ticks();
        }

        update_display();
        apply_opacity();
        show_display();
    }

    stop_animation();

    clear_display();

    // Filter out any errors from lossy IR transmission
    check_receive_buffer();
    
    // Construct a data structure out of the received data
    ir_update_data();
    
    // Apply the new data settings
    ir_set_data();
}

/* The clock is gracefully stopped. */
void reset_clock(void) {
    disable_usart();
    USART_flush();
    reset_updating_flag();
    reset_buffer_markers();
    clear_receive_buffer();
    
    // Handle EEPROM writing
    //if (eeprom_is_set()) {
        eeprom_update_data();
        eeprom_write_data();
    //}
    // Update the RTC time
    rtc_set_split_time(get_clock_hours(), get_clock_minutes(), get_clock_seconds());

    // Clear the new reset flag
    disable_new_reset();
}
