#include "effects.h"
#include "air001xx_hal.h"

extern uint8_t keyPressed = 0;

//return false then break
bool delay_ms(int ms)
{
  uint32_t time1;
  if(keyPressed == 1)
    return false;   //如果要切换模式，就不延时了。
  if(ms<10)
  {
    HAL_Delay(ms);
  }
  else
  {
    time1 = HAL_GetTick();
    while(HAL_GetTick() - time1 < ms)
    {
      if(keyPressed == 1)
        return false;//如果延时过程中触发按键，就不继续延时了。
    }
   }
  return true;
}

void resetPixels()
{
    for(uint16_t i=0; i<adafruit_neopixel.numPixels(); i++)
    {
        adafruit_neopixel.setPixelColor(i, 0);
    }
		adafruit_neopixel.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(uint8_t WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85)
    {
        return adafruit_neopixel.Color_rgb(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170)
    {
        WheelPos -= 85;
        return adafruit_neopixel.Color_rgb(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return adafruit_neopixel.Color_rgb(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for(uint16_t i=0; i<adafruit_neopixel.numPixels(); i++)
    {
        adafruit_neopixel.setPixelColor(i, c);
        adafruit_neopixel.show();
        //HAL_Delay(wait);
        if(!delay_ms(wait))
					break;
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
    for (int j=0; j<10; j++)//do 10 cycles of chasing
    {  
        for (int q=0; q < 3; q++)
        {
            for (uint16_t i=0; i < adafruit_neopixel.numPixels(); i=i+3)
            {
                adafruit_neopixel.setPixelColor(i+q, c);//turn every third pixel on
            }
            adafruit_neopixel.show();

            //HAL_Delay(wait);
            if(!delay_ms(wait))
					    return;

            for (uint16_t i=0; i < adafruit_neopixel.numPixels(); i=i+3)
            {
                adafruit_neopixel.setPixelColor(i+q, 0);//turn every third pixel off
            }
        }
    }
}

void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for(j=0; j<256; j++)
    {
        for(i=0; i<adafruit_neopixel.numPixels(); i++)
        {
            adafruit_neopixel.setPixelColor(i, Wheel((i+j) & 255));
        }
        adafruit_neopixel.show();
        //HAL_Delay(wait);
        if(!delay_ms(wait))
					break;
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for(j=0; j<256; j++)// 3 cycles of all colors on wheel
    { 
        for(i=0; i< adafruit_neopixel.numPixels(); i++)
        {
            adafruit_neopixel.setPixelColor(i, Wheel(((i * 256 / adafruit_neopixel.numPixels()) + j) & 255));
        }
        adafruit_neopixel.show();
        //HAL_Delay(wait);
        if(!delay_ms(wait))
					break;
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
    for (int j=0; j < 256; j++)// cycle all 256 colors in the wheel
    {     
        for (int q=0; q < 3; q++)
        {
            for (uint16_t i=0; i < adafruit_neopixel.numPixels(); i=i+3)
            {
                adafruit_neopixel.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
            }
            adafruit_neopixel.show();

            //HAL_Delay(wait);
            if(!delay_ms(wait))
					    return;
            for (uint16_t i=0; i < adafruit_neopixel.numPixels(); i=i+3)
            {
                adafruit_neopixel.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

void question_mark(){
   uint32_t color_array[]={adafruit_neopixel.Color_rgb(BRIGHTNESS, 0, 0),adafruit_neopixel.Color_rgb(BRIGHTNESS, BRIGHTNESS, 0), \
                        adafruit_neopixel.Color_rgb(0, BRIGHTNESS, 0),adafruit_neopixel.Color_rgb(0, BRIGHTNESS,BRIGHTNESS),\
                        adafruit_neopixel.Color_rgb(0, 0,BRIGHTNESS),adafruit_neopixel.Color_rgb(BRIGHTNESS, 0,BRIGHTNESS)};
   for(int j=0;j<6;j++){
    for(int i=0;i<3;i++)
    {
      adafruit_neopixel.setPixelColor(i+j*16, color_array[HAL_GetTick()%6]);
    }
    for(int i=7;i<11;i++)
    {
      adafruit_neopixel.setPixelColor(i+j*16, color_array[HAL_GetTick()%6]);
    }
    adafruit_neopixel.setPixelColor(12+j*16, color_array[HAL_GetTick()%6]);

   }
   adafruit_neopixel.show();   // Send the updated pixel colors to the hardware.
   if(!delay_ms(1000))
		 return;
}

void breathe_led_all(int wait){

      uint32_t rand_color=Wheel(HAL_GetTick()%255);

      for(int i=0; i<BRIGHTNESS; i++) { // For each pixel in pixels... 
        for(int n=0;n<WS2812_LINK_COUNT;n++){
          adafruit_neopixel.setPixelColor(n, rand_color);         //  Set pixel's color (in RAM)
        }
        adafruit_neopixel.setBrightness(i);
        adafruit_neopixel.show();                          //  Update pixels to match
        if(!delay_ms(wait))
					return;
      }  
      for(int i=BRIGHTNESS; i>0; i--) { // For each pixel in pixels...
        // pixels.setPixelColor(n, color);         //  Set pixel's color (in RAM)
        for(int n=0;n<WS2812_LINK_COUNT;n++){
          adafruit_neopixel.setPixelColor(n, rand_color);         //  Set pixel's color (in RAM)
        }
        adafruit_neopixel.setBrightness(i);
        adafruit_neopixel.show();                          //  Update pixels to match
        if(!delay_ms(wait))
					return;				
    }
   // pixels.setBrightness(BRIGHTLESS);  
}

void rectangle_random(){
   uint32_t random_color[]={adafruit_neopixel.Color_rgb(BRIGHTNESS, 0, 0),adafruit_neopixel.Color_rgb(BRIGHTNESS, BRIGHTNESS, 0), \
                        adafruit_neopixel.Color_rgb(0, BRIGHTNESS, 0),adafruit_neopixel.Color_rgb(0, BRIGHTNESS,BRIGHTNESS),\
                        adafruit_neopixel.Color_rgb(0, 0,BRIGHTNESS),adafruit_neopixel.Color_rgb(BRIGHTNESS, 0,BRIGHTNESS)};
   for(int j=0;j<6;j++){
    for(int i=0;i<5;i++)
    {
      adafruit_neopixel.setPixelColor(i+j*16, random_color[j]);
    }
    adafruit_neopixel.setPixelColor(7+j*16,random_color[j]);
    adafruit_neopixel.setPixelColor(8+j*16,random_color[j]);
    for(int i=11;i<16;i++)
    {
      adafruit_neopixel.setPixelColor(i+j*16, random_color[j]);
    }
   }
   adafruit_neopixel.show();   // Send the updated pixel colors to the hardware.
   if(!delay_ms(1000))
		 return;
}

void rectangle_color_flow(int wait){
  int i;
    for(int j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  
      for(int n=0;n<6;n++){
        for( i=0; i< 5; i++) {
          adafruit_neopixel.setPixelColor(i+n*16, Wheel(((i * 256 / 12) + j) & 255));
        }
        for(int k=11;k<16;k++,i++)
        {
           adafruit_neopixel.setPixelColor(k+n*16, Wheel(((i * 256 / 12) + j) & 255));
        }
        adafruit_neopixel.setPixelColor(8+n*16, Wheel(((i++ * 256 / 12) + j) & 255));
        adafruit_neopixel.setPixelColor(7+n*16, Wheel(((i * 256 / 12) + j) & 255));
      }
      adafruit_neopixel.show();
      if(!delay_ms(wait))
      {
        return;
      }
    }
}

#define COLOR_STEP 8
void face_color_flow(int wait){
  int i;
  
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
  
      for(int n=0;n<6;n++){
        for( i=0; i< 5; i++) {
          adafruit_neopixel.setPixelColor(i+n*16, Wheel((i*COLOR_STEP+ j) & 255));
        }
        for(int k=11;k<16;k++,i++)
        {
           adafruit_neopixel.setPixelColor(k+n*16, Wheel((i*COLOR_STEP + j) & 255));
        }
        for(int k=8;k>4;k--,i++)
        {
           adafruit_neopixel.setPixelColor(k+n*16, Wheel((i*COLOR_STEP + j) & 255));
        }
        adafruit_neopixel.setPixelColor(10+n*16, Wheel((i++*COLOR_STEP + j) & 255));
        adafruit_neopixel.setPixelColor(9+n*16, Wheel((i*COLOR_STEP + j) & 255));
      }
      adafruit_neopixel.show();
      if(!delay_ms(wait))
      {
        return;
      }
    }
}

void one_face_one_color_fixed()
{
   uint32_t color_array[]={adafruit_neopixel.Color_rgb(BRIGHTNESS, 0, 0),adafruit_neopixel.Color_rgb(BRIGHTNESS, BRIGHTNESS, 0), \
                        adafruit_neopixel.Color_rgb(0, BRIGHTNESS, 0),adafruit_neopixel.Color_rgb(0, BRIGHTNESS,BRIGHTNESS),\
                        adafruit_neopixel.Color_rgb(0, 0,BRIGHTNESS),adafruit_neopixel.Color_rgb(BRIGHTNESS, 0,BRIGHTNESS)};
  for(int j=0;j<6;j++){
    for(int i=0; i<16; i++) { 
      adafruit_neopixel.setPixelColor(i+j*16, color_array[j]);
    }
  }
  adafruit_neopixel.show();   // Send the updated pixel colors to the hardware.
  delay_ms(1000);
}


/* USER CODE END 0 */

void make_data()
{
   uint32_t color_array[]={adafruit_neopixel.Color_rgb(BRIGHTNESS, 0, 0),adafruit_neopixel.Color_rgb(BRIGHTNESS, BRIGHTNESS, 0), \
                        adafruit_neopixel.Color_rgb(0, BRIGHTNESS, 0),adafruit_neopixel.Color_rgb(0, BRIGHTNESS,BRIGHTNESS),\
                        adafruit_neopixel.Color_rgb(0, 0,BRIGHTNESS),adafruit_neopixel.Color_rgb(BRIGHTNESS, 0,BRIGHTNESS)};
  for(int j=0;j<6;j++){
    for(int i=0; i<16; i++) { 
      adafruit_neopixel.setPixelColor(i+j*16, color_array[j]);
    }
  }
  adafruit_neopixel.show();   // Send the updated pixel colors to the hardware.
  delay_ms(1000);
}