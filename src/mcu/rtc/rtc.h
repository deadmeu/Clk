#ifndef RTC_H_
#define RTC_H_

#include <avr/io.h>
#include <util/twi.h>
#include <avr/sfr_defs.h>

/*****************************************************************************
 * Human readble format for bit numbers / register descriptions.
 ****************************************************************************/

/*
 * The seconds register, RTCSEC.
 */
#define RTCSEC 0x00
#define SECONE 0
#define SECTEN 4
#define ST 7

/*
 * The minutes register, RTCMIN.
 */
#define RTCMIN 0x01
#define MINONE 0
#define MINTEN 4

/*
 * The hours register, RTCHOUR.
 */
#define RTCHOUR 0x02
#define HRONE 0
#define HRTEN 4
#define AM_PM 5
#define TIME_12_24 6

/*
 * Constants for setting AM/PM and 12/24 format in the hour register.
 */
#define AM 0
#define PM 1
#define TIME_12 1
#define TIME_24 0

/*****************************************************************************
 * Functions to utilise the i2c bus.
 ****************************************************************************/
void set_rtc_seconds(int seconds);
void set_rtc_minutes(int minutes);
void set_rtc_hours(int hours);

int read_rtc_seconds(void);
int read_rtc_minutes(void);
int read_rtc_hours(void);

#endif /* RTC_H_ */
