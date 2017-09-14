/*
* Light_WS2812 library example - RGB_blinky
*
* cycles one LED through red, green, blue
*
* This example is configured for a ATtiny85 with PLL clock fuse set and
* the WS2812 string connected to PB4.
*/

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "light_ws2812.h"

#define BLINK_DELAY 200

void blink(void);
void fade(void);
void red(void);
void green(void);
void blue(void);
void yellow(void);
void purple(void);
void sky_blue(void);

struct cRGB led[4];

int main(void)
{
	while(1) {
		blink();
		//fade();
	}
}

void blink(void) {
	//red();
	//green();
	blue();
	//yellow();
	//purple();
	sky_blue();
}

void red(void) {
	led[0].r=255;led[0].g=0;led[0].b=0;    // Write red to array
	led[1].r=0;led[1].g=0;led[1].b=0;    // Write red to array
	led[2].r=0;led[2].g=0;led[2].b=0;    // Write red to array
	led[3].r=0;led[3].g=0;led[3].b=0;    // Write red to array
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);                         // wait for 500ms.

	led[0].r=0;led[0].g=0;led[0].b=0;    // Write red to array
	led[1].r=255;led[1].g=0;led[1].b=0;    // Write red to array
	led[2].r=0;led[2].g=0;led[2].b=0;    // Write red to array
	led[3].r=0;led[3].g=0;led[3].b=0;    // Write red to array
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);                         // wait for 500ms.

	led[0].r=0;led[0].g=0;led[0].b=0;    // Write red to array
	led[1].r=0;led[1].g=0;led[1].b=0;    // Write red to array
	led[2].r=255;led[2].g=0;led[2].b=0;    // Write red to array
	led[3].r=0;led[3].g=0;led[3].b=0;    // Write red to array
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);                         // wait for 500ms.

	led[0].r=0;led[0].g=0;led[0].b=0;    // Write red to array
	led[1].r=0;led[1].g=0;led[1].b=0;    // Write red to array
	led[2].r=0;led[2].g=0;led[2].b=0;    // Write red to array
	led[3].r=255;led[3].g=0;led[3].b=0;    // Write red to array
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);                         // wait for 500ms.
}

void green(void) {
	led[0].r=0;led[0].g=255;led[0].b=0;			// green
	led[1].r=0;led[1].g=0;led[1].b=0;			// green
	led[2].r=0;led[2].g=0;led[2].b=0;			// green
	led[3].r=0;led[3].g=0;led[3].b=0;			// green
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;			// green
	led[1].r=0;led[1].g=255;led[1].b=0;			// green
	led[2].r=0;led[2].g=0;led[2].b=0;			// green
	led[3].r=0;led[3].g=0;led[3].b=0;			// green
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);
	led[0].r=0;led[0].g=0;led[0].b=0;			// green
	led[1].r=0;led[1].g=0;led[1].b=0;			// green
	led[2].r=0;led[2].g=255;led[2].b=0;			// green
	led[3].r=0;led[3].g=0;led[3].b=0;			// green
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);
	led[0].r=0;led[0].g=0;led[0].b=0;			// green
	led[1].r=0;led[1].g=0;led[1].b=0;			// green
	led[2].r=0;led[2].g=0;led[2].b=0;			// green
	led[3].r=0;led[3].g=255;led[3].b=0;			// green
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);
}

void blue(void) {
	led[0].r=0;led[0].g=0;led[0].b=255;		// blue
	led[1].r=0;led[1].g=0;led[1].b=0;		// blue
	led[2].r=0;led[2].g=0;led[2].b=0;		// blue
	led[3].r=0;led[3].g=0;led[3].b=0;		// blue
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;		// blue
	led[1].r=0;led[1].g=0;led[1].b=255;		// blue
	led[2].r=0;led[2].g=0;led[2].b=0;		// blue
	led[3].r=0;led[3].g=0;led[3].b=0;		// blue
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;		// blue
	led[1].r=0;led[1].g=0;led[1].b=0;		// blue
	led[2].r=0;led[2].g=0;led[2].b=255;		// blue
	led[3].r=0;led[3].g=0;led[3].b=0;		// blue
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;		// blue
	led[1].r=0;led[1].g=0;led[1].b=0;		// blue
	led[2].r=0;led[2].g=0;led[2].b=0;		// blue
	led[3].r=0;led[3].g=0;led[3].b=255;		// blue
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);
}

void yellow(void) {
	led[0].r=255;led[0].g=255;led[0].b=0;		
	led[1].r=0;led[1].g=0;led[1].b=0;	
	led[2].r=0;led[2].g=0;led[2].b=0;	
	led[3].r=0;led[3].g=0;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=255;led[1].g=255;led[1].b=0;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=0;led[3].g=0;led[3].b=0;	
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;	
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=255;led[2].g=255;led[2].b=0;
	led[3].r=0;led[3].g=0;led[3].b=0;	
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=255;led[3].g=255;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);
}

void purple(void) {
	led[0].r=255;led[0].g=0;led[0].b=255;
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=0;led[3].g=0;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=255;led[1].g=0;led[1].b=255;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=0;led[3].g=0;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=255;led[2].g=0;led[2].b=255;
	led[3].r=0;led[3].g=0;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=255;led[3].g=0;led[3].b=255;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);
}

void sky_blue(void) {
	led[0].r=0;led[0].g=100;led[0].b=235;
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=0;led[3].g=0;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=0;led[1].g=100;led[1].b=235;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=0;led[3].g=0;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=0;led[2].g=100;led[2].b=235;
	led[3].r=0;led[3].g=0;led[3].b=0;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

	led[0].r=0;led[0].g=0;led[0].b=0;
	led[1].r=0;led[1].g=0;led[1].b=0;
	led[2].r=0;led[2].g=0;led[2].b=0;
	led[3].r=0;led[3].g=100;led[3].b=235;
	ws2812_setleds(led,4);
	_delay_ms(BLINK_DELAY);

}

void fade(void) {
	int i;
	for (i = 0; i < 255; i++) {
		led[0].r=i;led[0].g=i;led[0].b=i;   
		led[1].r=i;led[1].g=i;led[1].b=i;  
		led[2].r=i;led[2].g=i;led[2].b=i; 
		led[3].r=i;led[3].g=i;led[3].b=i;
		ws2812_setleds(led,4);
		_delay_ms(5);
	}
	for (i = 255; i > 0; i--) {
		led[0].r=i;led[0].g=i;led[0].b=i;   
		led[1].r=i;led[1].g=i;led[1].b=i;  
		led[2].r=i;led[2].g=i;led[2].b=i; 
		led[3].r=i;led[3].g=i;led[3].b=i;
		ws2812_setleds(led,4);
		_delay_ms(5);
	}
	_delay_ms(100);
}
