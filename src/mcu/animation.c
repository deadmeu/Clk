/*
 * animation.c
 *
 * Written by Alex Subaric.
 */

#include "pixel_colour.h"

pcol_t frames_sunny[2][16] = { 
                                {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
                                SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
                                SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}, 
             
                                {SUN_YELLOW, SKY_BLUE,   SKY_BLUE,   SUN_YELLOW,
                                SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
                                SKY_BLUE,    SUN_YELLOW, SUN_YELLOW, SKY_BLUE,
                                SUN_YELLOW,  SKY_BLUE,   SKY_BLUE,   SUN_YELLOW}
                             };

pcol_t frames_cloudy[2][16] = { 
                                {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    WHITE,      WHITE,      SKY_BLUE,
                                WHITE,       WHITE,      WHITE,      WHITE,
                                SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}, 
       
                                {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    WHITE,      WHITE,      SKY_BLUE,
                                WHITE,       WHITE,      WHITE,      WHITE,
                                SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}
                             };

pcol_t frames_rainy[2][16] = { 
                                {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
                                CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
                                RAIN_BLUE,   SKY_BLUE,   RAIN_BLUE,  SKY_BLUE,
                                SKY_BLUE,    RAIN_BLUE,  SKY_BLUE,   RAIN_BLUE}, 
             
                                {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
                                CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
                                SKY_BLUE,    RAIN_BLUE,  SKY_BLUE,   RAIN_BLUE,
                                RAIN_BLUE,   SKY_BLUE,   RAIN_BLUE,  SKY_BLUE}
                             };

pcol_t frames_stormy[2][16] = { 
                                {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
                                CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
                                SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE}, 
                
                                {SKY_BLUE,   CLOUD_GREY, CLOUD_GREY, SKY_BLUE,
                                CLOUD_GREY,  CLOUD_GREY, CLOUD_GREY, CLOUD_GREY,
                                SKY_BLUE,    SUN_YELLOW, SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    SKY_BLUE,   SUN_YELLOW, SKY_BLUE}
                             };

pcol_t frames_windy[2][16] = { 
                                {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                RAIN_BLUE,   RAIN_BLUE,  SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    SKY_BLUE,   RAIN_BLUE,  RAIN_BLUE},
                
                                {SKY_BLUE,   SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                SKY_BLUE,    SKY_BLUE,   RAIN_BLUE,  RAIN_BLUE,
                                SKY_BLUE,    SKY_BLUE,   SKY_BLUE,   SKY_BLUE,
                                RAIN_BLUE,   RAIN_BLUE,  SKY_BLUE,   SKY_BLUE}
                             };

pcol_t *animations[5] = {frames_sunny,  frames_cloudy, frames_rainy,
                         frames_stormy, frames_windy};

