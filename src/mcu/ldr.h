/*
 * ldr.h
 *
 * Written by Alex Subaric.
 */

#ifndef LDR_H_
#define LDR_H_

#define MAX_OPACITY      100
#define MIN_OPACITY        5
#define AMBIENT_MULTIPLIER	0.5

void init_ldr(void);

uint8_t get_ldr_opacity(void);

#endif /* LDR_H_ */
