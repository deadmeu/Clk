#include <FastLED.h>

#define NUM_LEDS 3
#define DATA_PIN 8
#define RED 0
#define GREEN 1
#define BLUE 2

#define RED_GREEN 0
#define GREEN_BLUE 1
#define BLUE_RED 2

CRGB leds[NUM_LEDS];
/////////////////////////////////////////////////
///*
//Rainbow LED
//
//LED Rainbow Cycle Using sin()
//
//by: Chris Innanen (aka Nonsanity)
//*/
//// how fast to change
//// with 0.1 as the frequency, 64 gives a full cycle
//const double frequency = .005;
//const unsigned cycleLen = 1200;
//
//unsigned index = 0;
//
//////////////////////////////////////
int prev_time;
int cur_time;

int colours[] = {RED, GREEN, BLUE};
int state;

int saturation = 66;
int r,g,b = saturation;
int threshold = 2;
int multiplier = 4;
int cycle_state = RED_GREEN;
int seconds;

void setup() {
    Serial.begin(19200);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    prev_time = millis();
    cur_time = millis();
    state = RED;
    r=255;
    seconds=0;
}

void loop() {
    cur_time = millis();
//    Serial.print(r);
//    Serial.print(" ");
//    Serial.print(g);
//    Serial.print(" ");
//    Serial.println(b);

    //static uint8_t hue = 0;
    //FastLED.showColor(CHSV(hue, 255, 255)); 
    //if (cur_time - prev_time >= 10) { 
      //prev_time = cur_time;
      //hue++;
    //}

/////////////////PROTOTYPE CLOCK////////////////////
    if (cur_time - prev_time >= 1000) {
      prev_time = cur_time;
      seconds++;
      display_time();
      FastLED.show();
    }
    
////////////////////////////////////////////////////



    

//    if (cur_time - prev_time >= 1000 ) {
//      prev_time = cur_time;
//      change_state();
//    }

//    update_colour(0);
//    cycle(0);


///////////////////////////////////////////////////////////////////////////////////////
//    // tick our index into the rainbow
//    index++;
//    if ( index >= cycleLen )
//      index = 1;
//    
//    // use out-of-phase sin waves to create the rainbow
//    uint8_t r = sin( (frequency * index) + 0 ) * 127 + 128;
//    uint8_t g = sin( (frequency * index) + 2 ) * 127 + 128;
//    uint8_t b = sin( (frequency * index) + 4 ) * 127 + 128;
//
//    leds[0] = CRGB(r,g,b);
///////////////////////////////////////////////////////////////////////////////////////

    
}

//void cycle(int led) {
//  if (cur_time - prev_time >= threshold) {
//    prev_time = cur_time;
//    switch(cycle_state) {
//      case RED_GREEN:
//        if (g < 255) {
//          g += 1 + multiplier;
//        } else {
//          r -= 1 + multiplier;
//        }
//        if (r <= saturation) {
//          cycle_state = GREEN_BLUE;
//        }
//        break;
//      case GREEN_BLUE:
//        if (b < 255) {
//          b += 1 ;
//        } else {
//          g -= 1 + multiplier;
//        }
//        if (g <= saturation) {
//          cycle_state = BLUE_RED;
//        }
//        break;
//      case BLUE_RED:
//        if (r < 255) {
//          r += 1 + multiplier;
//        } else {
//          b -= 1 + multiplier;
//        }
//        if (b <= saturation) {
//          cycle_state = RED_GREEN;
//        }
//        break;
//    }
//  }
//
//  leds[0] = CRGB(r,g,b);
//  
//}

void update_colour(int led_number) {
  switch(state) {
    case RED:
      leds[led_number] = CRGB::Red; break;
    case GREEN:
      leds[led_number] = CRGB::DarkCyan; break;
    case BLUE:
      leds[led_number] = CRGB::Blue; break;
  }
}


void change_state() {
  state = (state+1) % 3;
}

void display_time() {
  int bit_val;

  for (int i = 0; i < NUM_LEDS; i++) {
    if (bitRead(seconds, i)) {
      leds[i] = CRGB::Red;
    } else {
      leds[i] = CRGB::Black;
    }
  }
}


///////////////////////////////////////RAINBOW//////////////////
//#include <FastLED.h>
//
//#define LED_PIN     8
//#define NUM_LEDS    1
//#define BRIGHTNESS  128
//#define LED_TYPE    WS2811
//#define COLOR_ORDER GRB
//
//CRGB leds[NUM_LEDS];
//
//// This example shows several ways to set up and use 'palettes' of colors
//// with FastLED.
//
//void setup() {
//  //delay( 3000 ); // power-up safety delay
//  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.setBrightness(  BRIGHTNESS );
//}
//
//
//void loop()
//{
//  randomSeed(millis());
//   
//  int wait=random(10,30);
//  int dim=random(4,6);
//  int max_cycles=8;
//  int cycles=random(1,max_cycles+1);
//   
//  rainbowCycle(6, 1, 2);
//}
//
//void rainbowCycle(int wait, int cycles, int dim) {
//
//  //loop several times with same configurations and same delay
//  for(int cycle=0; cycle < cycles; cycle++){
//  
//    byte dir=random(0,2);
//    int k=255;
//    
//    //loop through all colors in the wheel
//    for (int j=0; j < 256; j++,k--) { 
//      
//      if(k<0) {
//        k=255;
//      }
//      
//      //Set RGB color of each LED
//      for(int i=0; i<NUM_LEDS; i++) {
//        
//        CRGB ledColor = wheel(((i * 256 / NUM_LEDS) + (dir==0?j:k)) % 256,dim);        
//        leds[i]=ledColor;
//        
//      }
//      
//      FastLED.show();
//      FastLED.delay(wait);
//    }
//  }
//  
//}
//
//CRGB wheel(int WheelPos, int dim) {
//  CRGB color;
//  if (85 > WheelPos) {
//   color.r=0;
//   color.g=WheelPos * 3/dim;
//   color.b=(255 - WheelPos * 3)/dim;;
//  } 
//  else if (170 > WheelPos) {
//   color.r=WheelPos * 3/dim;
//   color.g=(255 - WheelPos * 3)/dim;
//   color.b=0;
//  }
//  else {
//   color.r=(255 - WheelPos * 3)/dim;
//   color.g=0;
//   color.b=WheelPos * 3/dim;
//  }
//  return color;
//}
