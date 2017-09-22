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
#define SECONE0 0
#define SECONE1 1
#define SECONE2 2
#define SECONE3 3
#define SECTEN0 4
#define SECTEN1 5
#define SECTEN2 6
#define ST 7

/*
 * The minutes register, RTCMIN.
 */
#define RTCMIN 0x01
#define MINONE0 0
#define MINONE1 1
#define MINONE2 2
#define MINONE3 3
#define MINTEN0 4
#define MINTEN1 5
#define MINTEN2 6

/*
 * The hours register, RTCHOUR.
 */
#define RTCHOUR 0x02
#define HRONE0 0
#define HRONE1 1
#define HRONE2 2
#define HRONE3 3
#define HRTEN0 4
#define HRTEN1 5
#define AM_PM 5
#define TIME_12_24 6

/*
 * Constants for setting AM/PM and 12/24 format in the hour register.
 */
#define AM 0
#define PM 1
#define TIME_12 1
#define TIME_24 0

#endif /* RTC_H_ */
