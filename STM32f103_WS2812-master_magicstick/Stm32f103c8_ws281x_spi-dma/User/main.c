#include "stm32f10x.h"

#include "delay.h"
#include "ws2812.h"
#include "ws2812s.h"


const char s[5];
int16_t i;
uint8_t fu_index[] = {7, 26, 27, 37, 38, 41, 42, 54, 55, 57, 60, 61, 68, 71, 74, 77, 78, 83, 84, 85, 88, 90, 91, 95, 96};//25
uint8_t fu_index2[] = {10, 22, 23, 39, 40, 43, 44, 52, 53, 56, 58, 59, 67, 71, 74, 77, 81, 82, 86, 87, 89, 92, 93, 94, 98, 99, 102, 105,
												106, 110, 111, 116, 117, 122, 123, 125, 127, 128, 129, 130, 133, 136, 137, 139, 140, 141, 142, 146, 147, 149, 150,
													153, 155, 157, 158, 159, 163, 166, 167, 170, 171, 172, 181, 182, 183, 184, 187, 188, 189, 200, 201, 203, 214, 217,
														218, 230, 231, 235};
int main(void)
{
  //usart1_init(115200);
  delay_init();

  ws281x_init();
	delay_ms(100);
	ws281xs_init();
	delay_ms(100);
 
  while(1)
  {
     // Some example procedures showing how to display to the pixels:
  //ws281x_colorWipe(ws281x_color(255, 0, 0), 50); // Red
  //ws281x_colorWipe(ws281x_color(0, 255, 0), 50); // Green
  //ws281x_colorWipe(ws281x_color(0, 0, 255), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  //ws281x_theaterChase(ws281x_color(127, 127, 127), 50); // White
  //ws281x_theaterChase(ws281x_color(127, 0, 0), 50); // Red
  //ws281x_theaterChase(ws281x_color(0, 0, 127), 50); // Blue

  //ws281x_rainbow(20);
  //ws281x_rainbowCycle(20);
  //ws281x_theaterChaseRainbow(200);
			
//			for(i = 0; i < PIXEL_NUM; ++i)
//		{
//			ws281x_setPixelColor(i, ws281x_color(0,250,0));
//			ws281x_show();
//			delay_ms(100);
//		}
		ws281x_256square(fu_index2);
		delay_ms(2000);
		ws281xs_256square(fu_index2);
  }
}
