#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>


#define NUM_LEDS 1
#define DATA_PIN 8
#define RED 0
#define GREEN 1
#define BLUE 2

CRGB leds[NUM_LEDS];

int prev_time;
int cur_time;

int colours[] = {RED, GREEN, BLUE};
int state;

void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    prev_time = millis();
    cur_time = millis();
    state = RED;
}

void loop() {
    cur_time = millis();

    if (cur_time - prev_time >= 1000 ) {
      prev_time = cur_time;
      change_state();
    }

    update_colour(0);
    

      FastLED.show();

    
}

void update_colour(int led_number) {
  switch(state) {
    case RED:
      leds[led_number] = CRGB(145, 255, 50); break;
    case GREEN:
      leds[led_number] = CRGB::DarkCyan; break;
    case BLUE:
      leds[led_number] = CRGB::Blue; break;
  }
}


void change_state() {
  state = (state+1) % 3;
}


