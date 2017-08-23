




#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

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

void splash_screen(void) {

}

void initialise_clock(void) {

}

void run_clock(void) {

}

void reset_clock(void) {
  while (1) { ; } //hang the system
}