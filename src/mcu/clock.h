/*
 * clock.h
 *
 * Written by Alex Subaric.
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "pixel_colour.h"
#include "unique_types.h"

#define RING_LEDS           12
#define GRID_LEDS           16
#define CONV_TIME(h, m, s)      (((uint32_t)h*(uint32_t)3600) + ((uint32_t)m*(uint32_t)60) + (uint32_t)s)

typedef enum { ANTE, POST } merid_t;

typedef enum {
    ANIMATION_FRAMES = 2,
    ANIMATION_LOOPS = 2
} anim_t;

void init_clock(void);
void enable_new_reset(void);
void disable_new_reset(void);
void apply_opacity(void);
void set_meridiem_colours(merid_t merid, pcol_t col);
void update_meridiem(void);
void play_animation(void);
void play_ir_animation(void);
void play_weather_animation(void);
void play_splash_animation(void);
void stop_animation(void);
void play_alarm_animation(void);
void play_alarm_sound(void);
void stop_alarm_sound(void);
void show_display(void);
void toggle_hour_marker(void);
void increment_seconds(void);
void disable_alarm(void);
void enable_alarm(void);
void disable_usart(void);
void disable_weather(void);
void enable_weather(void);
void update_playing_weather(void);
void disable_eeprom(void);
void enable_eeprom(void);
void reset_minute_flag(void);
void reset_alarm_flag(void);
void update_opacity(void);
void update_display(void);
void reset_redraw_flags(void);
void reset_ring_redraw(void);
void reset_grid_redraw(void);
void call_ring_redraw(void);
void call_grid_redraw(void);
void update_animation_frame(void);
void reset_time_changed_flag(void);
void enable_usart(void);
void update_new_minute_flag(void);
void clear_display(void);

wtype_t get_weather_one(void);
wtype_t get_weather_two(void);

uint8_t clock_time_changed(void);
uint8_t clock_update_time(void);
uint8_t set_time(uint32_t new_time);
uint8_t set_split_clock_time(uint8_t h, uint8_t m, uint8_t s);
uint8_t set_alarm_time(uint32_t new_time);
uint8_t set_split_alarm_time(uint8_t h, uint8_t m, uint8_t s);
uint8_t set_opacity(uint8_t new_opacity);
uint8_t set_weather(wtype_t weather_one, wtype_t weather_two);
uint8_t is_new_reset(void);
uint8_t alarm_is_set(void);
uint8_t eeprom_is_set(void);
uint8_t weather_is_set(void);
uint8_t usart_enabled(void);
uint8_t animation_is_playing(void);
uint8_t alarm_is_playing(void);
uint8_t splash_playing(void);
uint8_t reached_new_minute(void);
uint8_t reached_alarm_time(void);
uint8_t get_alarm_seconds(void);
uint8_t get_alarm_minutes(void);
uint8_t get_alarm_hours(void);
uint8_t get_clock_seconds(void);
uint8_t get_clock_minutes(void);
uint8_t get_clock_hours(void);
uint8_t redraw_ring_needed(void);
uint8_t redraw_grid_needed(void);

uint32_t get_time(void);

#endif /* CLOCK_H_ */
