/*
 * pixel_colour.h
 *
 * Written by Alex Subaric
 */

#ifndef PIXEL_COLOUR_H_
#define PIXEL_COLOUR_H_

// // Standard Colours
// #define BLACK       0  , 0  , 0  
// #define BLUE        0  , 0  , 255
// #define GREEN       0  , 255, 0  
// #define CYAN        0  , 255, 255
// #define RED         255, 0  , 0  
// #define PURPLE      255, 0  , 255
// #define YELLOW      255, 255, 0  
// #define WHITE       255, 255, 255

// // Custom Colours
// #define ORANGE      255, 150, 0  
// #define PINK        255, 0  , 150
// #define TEAL        0  , 128, 128
// #define LIME        150, 255, 0  
// #define SKY_BLUE    0  , 100, 255
// #define SUN_YELLOW  200, 220, 25 
// #define TOXIC_GREEN 180, 255, 0

typedef enum {
    /* Standard colours */
    BLACK       = 0x000000,
    BLUE        = 0x0000FF,
    GREEN       = 0x00FF00,
    CYAN        = 0x00FFFF,
    RED         = 0xFF0000,
    PURPLE      = 0xFF00FF,
    YELLOW      = 0xFFFF00,
    WHITE       = 0xFFFFFF,
    /* Custom colours */
    ORANGE      = 0xFF9600,
    PINK        = 0xFF0096,
    TEAL        = 0x008080,
    LIME        = 0x96FF00,
    SKY_BLUE    = 0x0064FF,
    SUN_YELLOW  = 0xC8DC19,
    TOXIC_GREEN = 0xB4FF00,
    CLOUD_GREY  = 0x82827F,
    RAIN_BLUE   = BLUE
} pcol_t;

#endif /* PIXEL_COLOUR_H_ */
