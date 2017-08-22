/*
 * Flashes RGB on 4 leds and fades from black to white.
 * Adapted from the light_ws2812 blinky example.
 * Written by Huy Nguyen.
 */

#ifndef F_CPU
#define F_CPU 16000000L
#endif

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

void blink(void);
void fade(void);

struct cRGB led[4];

int main(void)
{
	while(1) {
		blink();
		fade();
	}
}

void blink(void) {
	int i;
	for (i = 0; i < 2; i++) {
		led[0].r=255;led[0].g=00;led[0].b=0;    // Write red to array
		led[1].r=255;led[1].g=00;led[1].b=0;    // Write red to array
		led[2].r=255;led[2].g=00;led[2].b=0;    // Write red to array
		led[3].r=255;led[3].g=00;led[3].b=0;    // Write red to array
		ws2812_setleds(led,4);
		_delay_ms(500);                         // wait for 500ms.

		led[0].r=0;led[0].g=255;led[0].b=0;			// green
		led[1].r=0;led[1].g=255;led[1].b=0;			// green
		led[2].r=0;led[2].g=255;led[2].b=0;			// green
		led[3].r=0;led[3].g=255;led[3].b=0;			// green
		ws2812_setleds(led,4);
		_delay_ms(500);

		led[0].r=0;led[0].g=00;led[0].b=255;		// blue
		led[1].r=0;led[1].g=00;led[1].b=255;		// blue
		led[2].r=0;led[2].g=00;led[2].b=255;		// blue
		led[3].r=0;led[3].g=00;led[3].b=255;		// blue
		ws2812_setleds(led,4);
		_delay_ms(500);
	}
}

void fade(void) {
	int j;
	int i;
	for (j = 0; j < 4; j++) {
		for (i = 0; i < 255; i++) {
			led[0].r=i;led[0].g=i;led[0].b=i;   
			led[1].r=i;led[1].g=i;led[1].b=i;  
			led[2].r=i;led[2].g=i;led[2].b=i; 
			led[3].r=i;led[3].g=i;led[3].b=i;
			ws2812_setleds(led,4);
			_delay_ms(2);
		}
		for (i = 255; i > 0; i--) {
			led[0].r=i;led[0].g=i;led[0].b=i;   
			led[1].r=i;led[1].g=i;led[1].b=i;  
			led[2].r=i;led[2].g=i;led[2].b=i; 
			led[3].r=i;led[3].g=i;led[3].b=i;
			ws2812_setleds(led,4);
			_delay_ms(2);
		}
		_delay_ms(100);
	}
}
