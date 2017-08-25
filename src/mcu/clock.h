/*
 * clock.h
 *
 * Written by Alex Subaric.
 */

#ifndef CLOCK_H_
#define CLOCK_H_

void init_clock(void);
void set_time(uint32_t time);
void splash_off(void);
void tick_clock(void);
void update_display(void);

#endif /* CLOCK_H_ */
