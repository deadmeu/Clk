/*
 * project.c
 *
 * Written by Alex Subaric.
 */

#include <avr/io.h>

#include "clock.h"
#include "ir.h"
#include "ldr.h"
#include "ledarray.h"
#include "splash.h"
#include "timer.h"

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

void initialise_hardware(void);
void initialise_clock(void);
void run_clock(void);
void reset_clock(void);

int main(void) {
  initialise_hardware();
  splash_screen();
  splash_off();

  while(1) {
    initialise_clock();
    run_clock();
    reset_clock();
  }
}

void initialise_hardware(void) {
  
}

void initialise_clock(void) {

}

void run_clock(void) {

}

void reset_clock(void) {
  while (1) { ; } //hang the system
}
