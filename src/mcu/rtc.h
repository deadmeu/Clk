/*
 * rtc.h
 *
 * Written by Huy Nguyen.
 * Modified by Alex Subaric.
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

void init_rtc(void);

uint8_t rtc_started(void);
uint8_t rtc_running(void);
uint8_t rtc_set_split_time(uint8_t h, uint8_t m, uint8_t s);

uint32_t rtc_get_time(void);

#endif /* RTC_H_ */
