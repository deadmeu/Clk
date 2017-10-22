/*
 * animation.c
 *
 * Written by Alex Subaric.
 */

#include <stdint.h>

#include "animation.h"
#include "clock.h"
#include "pixel_colour.h"
#include "light_ws2812.h"
#include "unique_types.h"

pcol_t frames_sunny[ANIMATION_FRAMES][GRID_LEDS] = { 
    {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
    SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
    SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}, 

    {SUN_YELLOW, SKY_BLUE,   SKY_BLUE,   SUN_YELLOW,
    SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
    SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
    SUN_YELLOW,  SKY_BLUE,   SKY_BLUE,   SUN_YELLOW}
                                                    };

pcol_t frames_cloudy[ANIMATION_FRAMES][GRID_LEDS] = { 
    {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    WHITE,      WHITE,      SKY_BLUE,
    WHITE,       WHITE,      WHITE,      WHITE,
    SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}, 

    {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    WHITE,      WHITE,      SKY_BLUE,
    WHITE,       WHITE,      WHITE,      WHITE,
    SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}
                                                    };

pcol_t frames_rainy[ANIMATION_FRAMES][GRID_LEDS] = { 
    {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
    CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
    RAIN_BLUE,   SKY_BLUE,   RAIN_BLUE,  SKY_BLUE,
    SKY_BLUE,    RAIN_BLUE,  SKY_BLUE,   RAIN_BLUE}, 

    {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
    CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
    SKY_BLUE,    RAIN_BLUE,  SKY_BLUE,   RAIN_BLUE,
    RAIN_BLUE,   SKY_BLUE,   RAIN_BLUE,  SKY_BLUE}
                                                    };

pcol_t frames_stormy[ANIMATION_FRAMES][GRID_LEDS] = { 
    {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
    CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
    SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}, 

    {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
    CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
    SKY_BLUE,    SUN_YELLOW, SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    SKY_BLUE,   SUN_YELLOW, SKY_BLUE}
                                                    };

pcol_t frames_windy[ANIMATION_FRAMES][GRID_LEDS] = { 
    {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    CLOUD_GREY,  CLOUD_GREY, SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    SKY_BLUE,   CLOUD_GREY, CLOUD_GREY},

    {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    SKY_BLUE,    SKY_BLUE,   CLOUD_GREY, CLOUD_GREY,
    SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
    CLOUD_GREY,  CLOUD_GREY, SKY_BLUE,   SKY_BLUE}
                                                    };

pcol_t frames_ir[ANIMATION_FRAMES][GRID_LEDS] = { 
    {RED,  BLACK, RED,   BLACK,
    BLACK, RED,   BLACK, RED,
    RED,   BLACK, RED,   BLACK,
    BLACK, RED,   BLACK, RED},

    {BLACK, RED,   BLACK, RED,
    RED,    BLACK, RED,   BLACK,
    BLACK,  RED,   BLACK, RED,
    RED,    BLACK, RED,   BLACK}
                                                };

pcol_t frames_alarm[ANIMATION_FRAMES][GRID_LEDS] = { 
    {ORANGE,  BLACK, ORANGE,   BLACK,
    BLACK, ORANGE,   BLACK, ORANGE,
    ORANGE,   BLACK, ORANGE,   BLACK,
    BLACK, ORANGE,   BLACK, ORANGE},

    {BLACK, ORANGE,   BLACK, ORANGE,
    ORANGE,    BLACK, ORANGE,   BLACK,
    BLACK,  ORANGE,   BLACK, ORANGE,
    ORANGE,    BLACK, ORANGE,   BLACK}
                                                };

pcol_t frames_splash[ANIMATION_FRAMES][GRID_LEDS] = {
	{BLACK,  WHITE, WHITE,   BLACK,
		BLACK, BLACK,   BLACK, BLACK,
		YELLOW,   BLACK, BLACK,   YELLOW,
	BLACK, YELLOW,   YELLOW, BLACK},

	{BLACK,  BLACK, BLACK,   BLACK,
		BLACK, BLACK,   BLACK, BLACK,
		BLACK,   BLACK, BLACK,   BLACK,
	BLACK, BLACK,   BLACK, BLACK}
													};

uint8_t frame_count;

// Sets the frame counter to 0
void reset_frame_count(void) {
    frame_count = 0;
}

// Moves the frame counter to the next available frame
uint8_t incr_frame_count(void) {
    if (++frame_count > ANIMATION_FRAMES-1) {
        frame_count = 0;
    }
    return frame_count;
}

// Returns the colour for a pixel based on the weather and index
pcol_t get_frame_pixel(wtype_t weather, uint8_t i) {
    switch(weather) {
        case SUNNY : return frames_sunny[frame_count][i];
        case CLOUDY: return frames_cloudy[frame_count][i];
        case RAINY : return frames_rainy[frame_count][i];
        case STORMY: return frames_stormy[frame_count][i];
        case WINDY : return frames_windy[frame_count][i];
        default    : return BLACK;
    }
}

// Returns the colour for a pixel from the IR animation based on index
pcol_t get_ir_frame_pixel(uint8_t i) {
    return frames_ir[frame_count][i];
}

// Returns the colour for a pixel from the alarm animation based on index
pcol_t get_alarm_frame_pixel(uint8_t i) {
    return frames_alarm[frame_count][i];
}

// Returns the colour for a pixel from the splash animation based on index
pcol_t get_splash_frame_pixel(uint8_t i) {
    return frames_splash[frame_count][i];
}
