/*
 * clock.h
 *
 * Written by Alex Subaric.
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void init_clock(void);
void apply_opacity(void);
void play_weather_animation(void);
void stop_weather_animation(void);
void set_time(uint32_t time);
void show_display(void);
void toggle_hour_marker(void);
void splash_off(void);
void increment_seconds(void);
void update_display(void);

uint8_t animation_playing(void);
uint8_t reached_new_minute(void);

#endif /* CLOCK_H_ */
