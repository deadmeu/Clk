/*
 * clock.h
 *
 * Written by Alex Subaric.
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "pixel_colour.h"

typedef enum { ANTE, POST } merid_t;

void init_clock(void);
void apply_opacity(void);
void set_meridiem_colours(merid_t merid, pcol_t col);
void update_meridiem(void);
void play_weather_animation(void);
void stop_weather_animation(void);
void play_alarm_sound(void);
void stop_alarm_sound(void);
void show_display(void);
void toggle_hour_marker(void);
void splash_off(void);
void increment_seconds(void);
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

uint8_t set_time(uint32_t time);
uint8_t set_alarm_time(uint32_t new_time);
uint8_t set_opacity(uint8_t new_opacity);
uint8_t set_weather(uint8_t weather1_type, uint8_t weather2_type);
uint8_t alarm_is_set(void);
uint8_t weather_is_set(void);
uint8_t animation_is_playing(void);
uint8_t alarm_is_playing(void);
uint8_t reached_new_minute(void);
uint8_t reached_alarm_time(void);
uint8_t get_clock_seconds(void);
uint8_t get_clock_minutes(void);
uint8_t get_clock_hours(void);
uint8_t redraw_ring_needed(void);
uint8_t redraw_grid_needed(void);

uint32_t get_time(void);

#endif /* CLOCK_H_ */
