#ifndef	__EFFECTS_H
#define	__EFFECTS_H

#include "stdint.h"
#include "Adafruit_NeoPixel.h"

/***********************************************
 * WS2812ÊýÁ¿
************************************************/
#define WS2812_LINK_COUNT   32 

#define COLOR_STEP 8
#define BRIGHTNESS 100

//return false then break
bool delay_ms(int ms);

void resetPixels();

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos);

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait);

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait);

void rainbow(uint8_t wait);

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait);

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait);

void question_mark();

void breathe_led_all(int wait);

void rectangle_random();

void rectangle_color_flow(int wait);

void face_color_flow(int wait);

void one_face_one_color_fixed();


/* USER CODE END 0 */

void make_data();

#endif