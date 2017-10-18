/*
 * rtc.h
 *
 * Written by Huy Nguyen.
 * Modified by Alex Subaric.
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

typedef enum {
    RTCSEC_ADDR,
    RTCMIN_ADDR,
    RTCHOUR_ADDR,
    RTCWKDAY_ADDR
} RTCADDR;

typedef enum {
    ONE0_BIT,
    ONE1_BIT,
    ONE2_BIT,
	ONE3_BIT,
    TEN0_BIT,
    TEN1_BIT,
    TEN2_BIT,
    TIMEFORMAT_BIT = 6,
    ST_BIT = 7
} TIMEREG;

typedef enum {
    WKDAY0_BIT,
    WKDAY1_BIT,
    WKDAY2_BIT,
    VBATEN_BIT,
    PWRFAIL_BIT,
    OSCRUN_BIT
} WKDAYREG;

void init_rtc(void);

uint8_t rtc_started(void);
uint8_t rtc_running(void);
uint8_t rtc_set_split_time(uint8_t h, uint8_t m, uint8_t s);

uint32_t rtc_get_time(void);

#endif /* RTC_H_ */
