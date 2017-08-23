#include "clock.h"

uint8_t is_splash;

void init_clock(void) {
  ledarray_clear();

  is_splash = 1;
}

void splash_off(void) {
  is_splash = 0;
}