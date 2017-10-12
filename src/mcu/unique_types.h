/*
 * pixel_colour.h
 *
 * Written by Alex Subaric
 */

#ifndef UNIQUE_TYPES_H_
#define UNIQUE_TYPES_H_

#define MAX_TIME            86399L    // 23:59:59 converted to seconds.
#define MID_DAY_TIME        43200L    // 12:00:00 (midday) converted to seconds
#define MIN_HOUR            0
#define MIN_MINUTE          0
#define MIN_SECOND          0
#define MAX_HOUR            23
#define MAX_MINUTE          59
#define MAX_SECOND          59

typedef enum { SUNNY, CLOUDY, RAINY, STORMY, WINDY } wtype_t;

#endif /* UNIQUE_TYPES_H_ */
