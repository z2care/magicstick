#include "Adafruit_NeoPixel.h"
#include "stdlib.h"
#include "string.h"

//static uint8_t pixel_buffer[96*3];

// ������������Adafruit_NeoPixel��֮�������ģ���ΪһЩ���������Ҫ������C++11 conexpr�ؼ��ֵ���ʽ��������
#if EN_sin8==1
/* ����8λ�޷������Ҳ�(0-255)��PROGMEM(�����ڴ�)��.
   ���˴���θ��Ʋ�ճ����Python REPL������������:
    import math
    for x in range(256):
        print("{:3},".format(int((math.sin(x/128.0*math.pi)+1.0)*127.5+0.5))),
        if x&15 == 15: print
*/
static const uint8_t _NeoPixelSineTable[256] = {
    128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170,
    173, 176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211,
    213, 215, 218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240,
    241, 243, 244, 245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254,
    254, 255, 255, 255, 255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251,
    250, 250, 249, 248, 246, 245, 244, 243, 241, 240, 238, 237, 235, 234, 232,
    230, 228, 226, 224, 222, 220, 218, 215, 213, 211, 208, 206, 203, 201, 198,
    196, 193, 190, 188, 185, 182, 179, 176, 173, 170, 167, 165, 162, 158, 155,
    152, 149, 146, 143, 140, 137, 134, 131, 128, 124, 121, 118, 115, 112, 109,
    106, 103, 100, 97,  93,  90,  88,  85,  82,  79,  76,  73,  70,  67,  65,
    62,  59,  57,  54,  52,  49,  47,  44,  42,  40,  37,  35,  33,  31,  29,
    27,  25,  23,  21,  20,  18,  17,  15,  14,  12,  11,  10,  9,   7,   6,
    5,   5,   4,   3,   2,   2,   1,   1,   1,   0,   0,   0,   0,   0,   0,
    0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,   10,  11,
    12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,  37,
    40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,
    79,  82,  85,  88,  90,  93,  97,  100, 103, 106, 109, 112, 115, 118, 121,
    124};
#endif

#if (EN_gamma8==1) || (EN_gamma32==1)
/* 8λ٤��У����
   ���˴���θ��Ʋ�ճ����Python REPL������������:
    import math
    gamma=2.6
    for x in range(256):
        print("{:3},".format(int(math.pow((x)/255.0,gamma)*255.0+0.5))),
        if x&15 == 15: print
*/
static const uint8_t _NeoPixelGammaTable[255] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
    1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
    3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
    6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
    11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
    17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
    25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
    36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
    49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
    64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
    82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
    103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
    127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
    154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
    184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
    255};
#endif

/*!
  @brief   �������������NeoPixel�ܽ�.
*/
static void Adafruit_NeoPixel_begin(void)
{
    adafruit_neopixel.begun = true;
}

/*!
  @brief   ������ǰ������Adafruit_NeoPixel��������ĳ��ȡ�
           �����ݱ��ͷţ������ݱ������
           ���ű�ź����ظ�ʽ���䡣
  @param   n  �µ��������ȣ�������Ϊ��λ��
  @note    �˺��������ã��˴��������ڿ������ڵ������ľ���Ŀ��
           ����ĿӦ�ø��ô��е�һ�����캯���﷨(length, pin,type)�ġ�new���ؼ��֡�
*/
static void Adafruit_NeoPixel_updateLength(uint16_t n)
{
    free(adafruit_neopixel.pixels); // �ͷ��������� (if any)

    // ����������--ע�⣺�������ض������
    adafruit_neopixel.numBytes = n * ((adafruit_neopixel.wOffset == adafruit_neopixel.rOffset) ? 3 : 4);
    adafruit_neopixel.pixels = (uint8_t *)malloc(adafruit_neopixel.numBytes);
    if (adafruit_neopixel.pixels)
    {
        memset(adafruit_neopixel.pixels, 0, adafruit_neopixel.numBytes);
        adafruit_neopixel.numLEDs = n;
    }
    else
    {
        adafruit_neopixel.numLEDs = adafruit_neopixel.numBytes = 0;
    }
}

/*!
  @brief   ������ǰ������Adafruit_NeoPixel������������ظ�ʽ��
           �����ʽ��RGB����֮һ����ΪRGBW����(��RGBW��RGB)��������ݽ����ͷţ������ݽ��������
           ���򣬾����ݽ�������RAM�У����Ҳ�����������Ϊ�¸�ʽ����˽���ʹ��Clear()���к�������
  @param   t  Pixel type -- ��Adafruit_NeoPixel.h�ж����neo_*��������һ��
           ���磬����NeoPixels��NEO_GRB+NEO_KHZ800��Ҫ800 kHz(Vs 400 KHz)��������
           ��ɫ�ֽ���ÿ���ص���ɫ����ɫ����ɫ˳���ʾ��
  @note    �˺��������ã��˴��������ڿ������ڵ������ľ���Ŀ��
           ����ĿӦ�ø��ô��е�һ�����캯���﷨(length, pin, type)�ġ�new���ؼ��֡�
*/
static void Adafruit_NeoPixel_updateType(neoPixelType t)
{
    bool oldThreeBytesPerPixel = (adafruit_neopixel.wOffset == adafruit_neopixel.rOffset); // false if RGBW

    adafruit_neopixel.wOffset = (t >> 6) & 0x03; // See notes in header file
    adafruit_neopixel.rOffset = (t >> 4) & 0x03; // regarding R/G/B/W offsets
    adafruit_neopixel.gOffset = (t >> 2) & 0x03;
    adafruit_neopixel.bOffset = t & 0x03;

    // ���ÿ�����ֽ����Ѹ���(������ǰ�ѷ�����������)�������·��䵽�µĴ�С��������������ݡ�
    if (adafruit_neopixel.pixels)
    {
        bool newThreeBytesPerPixel = (adafruit_neopixel.wOffset == adafruit_neopixel.rOffset);
        if (newThreeBytesPerPixel != oldThreeBytesPerPixel)
            Adafruit_NeoPixel_updateLength(adafruit_neopixel.numLEDs);
    }
}

/*!
  @brief   ��RAM�е��������ݴ��䵽NeoPixels��
  @note    �ڴ�����ܹ��ϣ��жϱ���ʱ���ã���ʵ����ȷ��NeoPixel�ź�ʱ��
           ����ζ����Ҫ�жϵ�Arduino Millis()��micros()������ÿ�ε��øú���ʱ
           ���ᶪʧ��С��ʱ����(ÿRGB���ش�Լ30΢�룬RGBW���ش�Լ40΢��)��
           �������û�м򵥵Ľ���취��������һЩר�ŵ���������׵Ŀ⣬
           ����ʹ�÷ǳ��ض����豸����Χ�豸���������
*/
static void Adafruit_NeoPixel_show(void)
{

    if ((!adafruit_neopixel.pixels) || (!adafruit_neopixel.NeoPixel_op))
        return;

    adafruit_neopixel.NeoPixel_op(adafruit_neopixel.pixels,adafruit_neopixel.numBytes);
}

#if EN_setPixelColor_rgb==1
/*!
  @brief   ʹ�õ����ĺ�ɫ����ɫ����ɫ�����������ص���ɫ�����ʹ��RGBW���أ����ɫ������Ϊ0��
  @param   n  Pixel index, starting from 0.
  @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
  @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
  @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
*/
static void Adafruit_NeoPixel_setPixelColor_rgb(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t *p;
    
    if (n < adafruit_neopixel.numLEDs)
    {
        if (adafruit_neopixel.brightness)// �����setBright()�е�ע��
        {
            r = (r * adafruit_neopixel.brightness) >> 8;
            g = (g * adafruit_neopixel.brightness) >> 8;
            b = (b * adafruit_neopixel.brightness) >> 8;
        }
        
        if (adafruit_neopixel.wOffset == adafruit_neopixel.rOffset)// ��RGB���͵�����
        {
            p = &adafruit_neopixel.pixels[n * 3];// ÿ����3���ֽ�
        }
        else// ��WRGB���͵�����
        {
            p = &adafruit_neopixel.pixels[n * 4];// ÿ����4�ֽ�
            p[adafruit_neopixel.wOffset] = 0;// But only R,G,B passed -- set W to 0
        }
        p[adafruit_neopixel.rOffset] = r; // R,G,B ʼ�մ洢
        p[adafruit_neopixel.gOffset] = g;
        p[adafruit_neopixel.bOffset] = b;
    }
}
#endif

#if EN_setPixelColor_rgbw==1
/*!
  @brief   ʹ�õ����ĺ졢�̡������׷����������ص���ɫ(��������RGBW NeoPixels)��
  @param   n  Pixel index, starting from 0.
  @param   r  Red brightness, 0 = minimum (off), 255 = maximum.
  @param   g  Green brightness, 0 = minimum (off), 255 = maximum.
  @param   b  Blue brightness, 0 = minimum (off), 255 = maximum.
  @param   w  White brightness, 0 = minimum (off), 255 = maximum, ignored
              if using RGB pixels.
*/
static void Adafruit_NeoPixel_setPixelColor_rgbw(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    uint8_t *p;
    
    if (n < adafruit_neopixel.numLEDs)
    {
        if (adafruit_neopixel.brightness)// �����setBright()�е�ע��
        {
            r = (r * adafruit_neopixel.brightness) >> 8;
            g = (g * adafruit_neopixel.brightness) >> 8;
            b = (b * adafruit_neopixel.brightness) >> 8;
            w = (w * adafruit_neopixel.brightness) >> 8;
        }
        
        if (adafruit_neopixel.wOffset == adafruit_neopixel.rOffset)// ��RGB���͵�����
        {
            p = &adafruit_neopixel.pixels[n * 3];// ÿ����3���ֽ�
        }
        else// ��WRGB���͵�����
        {
            p = &adafruit_neopixel.pixels[n * 4];// ÿ����4�ֽ�
            p[adafruit_neopixel.wOffset] = w; // Store W
        }
        p[adafruit_neopixel.rOffset] = r; // Store R,G,B
        p[adafruit_neopixel.gOffset] = g;
        p[adafruit_neopixel.bOffset] = b;
    }
}
#endif

#if EN_setPixelColor==1
/*!
  @brief   ʹ��32λRGB��RGBWֵ�������ص���ɫ��
  @param   n  Pixel index, starting from 0.
  @param   c  32-bit color value. Most significant byte is white (for RGBW
              pixels) or ignored (for RGB pixels), next is red, then green,
              and least significant byte is blue.
*/
static void Adafruit_NeoPixel_setPixelColor(uint16_t n, uint32_t c)
{
    uint8_t w;
    uint8_t *p, r = (uint8_t)(c >> 16), g = (uint8_t)(c >> 8), b = (uint8_t)c;
    
    if (n < adafruit_neopixel.numLEDs)
    {
        if (adafruit_neopixel.brightness)// �����setBright()�е�ע��
        {
            r = (r * adafruit_neopixel.brightness) >> 8;
            g = (g * adafruit_neopixel.brightness) >> 8;
            b = (b * adafruit_neopixel.brightness) >> 8;
        }
        if (adafruit_neopixel.wOffset == adafruit_neopixel.rOffset)
        {
            p = &adafruit_neopixel.pixels[n * 3];
        }
        else
        {
            p = &adafruit_neopixel.pixels[n * 4];
            w = (uint8_t)(c >> 24);
            p[adafruit_neopixel.wOffset] = adafruit_neopixel.brightness ? ((w * adafruit_neopixel.brightness) >> 8) : w;
        }
        p[adafruit_neopixel.rOffset] = r;
        p[adafruit_neopixel.gOffset] = g;
        p[adafruit_neopixel.bOffset] = b;
    }
}
#endif

#if EN_fill==1
/*!
  @brief   ����ɫ���ȫ���򲿷�NeoPixel������
  @param   c      32-bit color value. Most significant byte is white (for
                  RGBW pixels) or ignored (for RGB pixels), next is red,
                  then green, and least significant byte is blue. If all
                  arguments are unspecified, this will be 0 (off).
  @param   first  Index of first pixel to fill, starting from 0. Must be
                  in-bounds, no clipping is performed. 0 if unspecified.
  @param   count  Number of pixels to fill, as a positive value. Passing
                  0 or leaving unspecified will fill to end of strip.
*/
static void Adafruit_NeoPixel_fill(uint32_t c, uint16_t first, uint16_t count)
{
    uint16_t i, end;

    if (first >= adafruit_neopixel.numLEDs)
    {
        return; // �����һ��LED��������ĩ�ˣ�������ִ���κβ���
    }

    // ����Ҫ�������һ������֮�������
    if (count == 0)
    {
        end = adafruit_neopixel.numLEDs;//���������ĩβ
    }
    else
    {
        end = first + count;// ȷ��ѭ�����ᳬ�����һ������
        if (end > adafruit_neopixel.numLEDs)
            end = adafruit_neopixel.numLEDs;
    }

    for (i = first; i < end; i++)
    {
        Adafruit_NeoPixel_setPixelColor(i, c);
    }
}
#endif

#if EN_ColorHSV==1
/*!
  @brief   ��ɫ�������ͶȺ�ֵת��Ϊ�����32λRGB��ɫ������ɫ���Դ��ݸ�setPixelColor()��������RGB���ݵĺ�����
  @param   hue  An unsigned 16-bit value, 0 to 65535, representing one full
                loop of the color wheel, which allows 16-bit hues to "roll
                over" while still doing the expected thing (and allowing
                more precision than the wheel() function that was common to
                prior NeoPixel examples).
  @param   sat  Saturation, 8-bit value, 0 (min or pure grayscale) to 255
                (max or pure hue). Default of 255 if unspecified.
  @param   val  Value (brightness), 8-bit value, 0 (min / black / off) to
                255 (max or full brightness). Default of 255 if unspecified.
  @return  Packed 32-bit RGB with the most significant byte set to 0 -- the
           white element of WRGB pixels is NOT utilized. Result is linearly
           but not perceptually correct, so you may want to pass the result
           through the gamma32() function (or your own gamma-correction
           operation) else colors may appear washed out. This is not done
           automatically by this function because coders may desire a more
           refined gamma-correction function than the simplified
           one-size-fits-all operation of gamma32(). Diffusing the LEDs also
           really seems to help when using low-saturation colors.
*/
static uint32_t Adafruit_NeoPixel_ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
{
    uint8_t r, g, b;
    uint32_t v1;
    uint16_t s1;
    uint8_t s2;
  // Remap 0-65535 to 0-1529. Pure red is CENTERED on the 64K rollover;
  // 0 is not the start of pure red, but the midpoint...a few values above
  // zero and a few below 65536 all yield pure red (similarly, 32768 is the
  // midpoint, not start, of pure cyan). The 8-bit RGB hexcone (256 values
  // each for red, green, blue) really only allows for 1530 distinct hues
  // (not 1536, more on that below), but the full unsigned 16-bit type was
  // chosen for hue so that one's code can easily handle a contiguous color
  // wheel by allowing hue to roll over in either direction.
  hue = (hue * 1530L + 32768) / 65536;
  // Because red is centered on the rollover point (the +32768 above,
  // essentially a fixed-point +0.5), the above actually yields 0 to 1530,
  // where 0 and 1530 would yield the same thing. Rather than apply a
  // costly modulo operator, 1530 is handled as a special case below.

  // So you'd think that the color "hexcone" (the thing that ramps from
  // pure red, to pure yellow, to pure green and so forth back to red,
  // yielding six slices), and with each color component having 256
  // possible values (0-255), might have 1536 possible items (6*256),
  // but in reality there's 1530. This is because the last element in
  // each 256-element slice is equal to the first element of the next
  // slice, and keeping those in there this would create small
  // discontinuities in the color wheel. So the last element of each
  // slice is dropped...we regard only elements 0-254, with item 255
  // being picked up as element 0 of the next slice. Like this:
  // Red to not-quite-pure-yellow is:        255,   0, 0 to 255, 254,   0
  // Pure yellow to not-quite-pure-green is: 255, 255, 0 to   1, 255,   0
  // Pure green to not-quite-pure-cyan is:     0, 255, 0 to   0, 255, 254
  // and so forth. Hence, 1530 distinct hues (0 to 1529), and hence why
  // the constants below are not the multiples of 256 you might expect.

  // Convert hue to R,G,B (nested ifs faster than divide+mod+switch):
    if (hue < 510)// Red to Green-1
    {
        b = 0;
        if (hue < 255)//  Red to Yellow-1
        {
            r = 255;
            g = hue;// g = 0 to 254
        }
        else//  Yellow to Green-1
        {
            r = 510 - hue; // r = 255 to 1
            g = 255;
        }
    }
    else if (hue < 1020)// Green to Blue-1
    {
        r = 0;
        if (hue < 765)// Green to Cyan-1
        {
            g = 255;
            b = hue - 510;  // b = 0 to 254
        }
        else// Cyan to Blue-1
        {
            g = 1020 - hue; // g = 255 to 1
            b = 255;
        }
    }
    else if (hue < 1530)// Blue to Red-1
    {
        g = 0;
        if (hue < 1275)// Blue to Magenta-1
        {
            r = hue - 1020; // r = 0 to 254
            b = 255;
        }
        else//   Magenta to Red-1
        {
            r = 255;
            b = 1530 - hue; // b = 255 to 1
        }
    }
    else// Last 0.5 Red (quicker than % operator)
    {
        r = 255;
        g = b = 0;
    }

    // �����ͶȺ�ֵӦ����R��G��B�����Ϊ32λ�����
    v1 = 1 + val;  // 1 to 256; allows >>8 instead of /255
    s1 = 1 + sat;  // 1 to 256; same reason
    s2 = 255 - sat; // 255 to 0
    return ((((((r * s1) >> 8) + s2) * v1) & 0xff00) << 8) |
           (((((g * s1) >> 8) + s2) * v1) & 0xff00) |
           (((((b * s1) >> 8) + s2) * v1) >> 8);
}
#endif

#if EN_getPixelColor==1
/*!
  @brief   ��ѯ��ǰ���õ����ص���ɫ��
  @param   n  Index of pixel to read (0 = first).
  @return  'Packed' 32-bit RGB or WRGB value. Most significant byte is white
           (for RGBW pixels) or 0 (for RGB pixels), next is red, then green,
           and least significant byte is blue.
  @note    If the strip brightness has been changed from the default value
           of 255, the color read from a pixel may not exactly match what
           was previously written with one of the setPixelColor() functions.
           This gets more pronounced at lower brightness levels.
*/
static uint32_t Adafruit_NeoPixel_getPixelColor(uint16_t n)
{
    uint8_t *p;
    
    if (n >= adafruit_neopixel.numLEDs)
        return 0; // Out of bounds, return no color.
    
    if (adafruit_neopixel.wOffset == adafruit_neopixel.rOffset)// Is RGB-type device
    {
        p = &adafruit_neopixel.pixels[n * 3];
        if (adafruit_neopixel.brightness)
        {
      // �洢����ɫ��setBright()��ȡ��
      // ����ֵ��ͼ��С������������ɫʱʹ�õ�ԭʼ24λֵ�Ľ���ֵ�������ǻ����һЩ����--��Щλ��ȫ��ʧ�ˡ�
      // �����ڵ����ȼ���ʱ��Ϊ���ԡ�
        return (((uint32_t)(p[adafruit_neopixel.rOffset] << 8) / adafruit_neopixel.brightness) << 16) |
               (((uint32_t)(p[adafruit_neopixel.gOffset] << 8) / adafruit_neopixel.brightness) << 8) |
               ((uint32_t)(p[adafruit_neopixel.bOffset] << 8) / adafruit_neopixel.brightness);
        }
        else// δ�������ȵ���--���ء�RAW����ɫ
        {
            return ((uint32_t)p[adafruit_neopixel.rOffset] << 16) | ((uint32_t)p[adafruit_neopixel.gOffset] << 8) |
                    (uint32_t)p[adafruit_neopixel.bOffset];
        }
    }
    else// Is RGBW-type device
    {
        p = &adafruit_neopixel.pixels[n * 4];
        if (adafruit_neopixel.brightness)// ����������ɫ
        {
            return (((uint32_t)(p[adafruit_neopixel.wOffset] << 8) / adafruit_neopixel.brightness) << 24) |
                    (((uint32_t)(p[adafruit_neopixel.rOffset] << 8) / adafruit_neopixel.brightness) << 16) |
                    (((uint32_t)(p[adafruit_neopixel.gOffset] << 8) / adafruit_neopixel.brightness) << 8) |
                    ((uint32_t)(p[adafruit_neopixel.bOffset] << 8) / adafruit_neopixel.brightness);
        }
        else// ����ԭʼ��ɫ
        {
            return ((uint32_t)p[adafruit_neopixel.wOffset] << 24) | ((uint32_t)p[adafruit_neopixel.rOffset] << 16) |
                   ((uint32_t)p[adafruit_neopixel.gOffset] << 8) | (uint32_t)p[adafruit_neopixel.bOffset];
        }
    }
}
#endif

/*!
  @brief   Adjust output brightness. Does not immediately affect what's
           currently displayed on the LEDs. The next call to show() will
           refresh the LEDs at this level.
  @param   b  Brightness setting, 0=minimum (off), 255=brightest.
  @note    This was intended for one-time use in one's setup() function,
           not as an animation effect in itself. Because of the way this
           library "pre-multiplies" LED colors in RAM, changing the
           brightness is often a "lossy" operation -- what you write to
           pixels isn't necessary the same as what you'll read back.
           Repeated brightness changes using this function exacerbate the
           problem. Smart programs therefore treat the strip as a
           write-only resource, maintaining their own state to render each
           frame of an animation, not relying on read-modify-write.
*/
static void Adafruit_NeoPixel_setBrightness(uint8_t b)
{
    uint8_t c, *ptr = adafruit_neopixel.pixels, oldBrightness = adafruit_neopixel.brightness - 1; // �Ծ�����ֵ���н��
    uint16_t scale;
    uint16_t i;
    
    // �洢������ֵ�봫�ݵ�ֵ��ͬ��
    // ������Ժ��ʵ�����ż��㣬������п��ٵ�8x8λ�˷���ȡMSB��
    // ��Brightness����uint8_t���������1���ܻ�(����)��ת...
    // ����0=�������(��ɫֵ��������˼���ͣ�û�����ű���)��1=��С����(��)��255=�Ե���������ȡ�
    uint8_t newBrightness = b + 1;
    if (newBrightness != adafruit_neopixel.brightness)
    {
        // ��֮ǰ��ֵ��ȣ����ȷ����˱仯--���µ���RAM���������ݵı�������һ���̿��ܻ��С���ġ����ر�������������ʱ��
        // WS2811/WS2812�����е�ʱ�����ζ��û���㹻�Ŀ��������������ݷ���ʱ��ִ̬���������š�
        // ��ˣ����Ƕ�RAM�����е���ɫ���ݽ��б����������������(����ͼ������Ҳ���ڴ����ȼ����¹���)��
        // �������������������ߣ���ô�����������޵Ĳ���(����)���������ŵİ汾�н��ǳ����ԡ�
        // ���ڷ��ƻ��Ը��ģ���Ҫ���³����������������ݡ� C'est la vie.
        if (oldBrightness == 0)
            scale = 0; // ����/0
        else if (b == 255)
            scale = 65535 / oldBrightness;
        else
            scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;

        for (i = 0; i < adafruit_neopixel.numBytes; i++)
        {
            c = *ptr;
            *ptr++ = (c * scale) >> 8;
        }
        adafruit_neopixel.brightness = newBrightness;
    }
}

#if EN_getBrightness==1
/*!
  @brief   ������һ�����õ���������ֵ��
  @return  Brightness value: 0 = minimum (off), 255 = maximum.
*/
static uint8_t Adafruit_NeoPixel_getBrightness(void)
{
    return adafruit_neopixel.brightness - 1;
}
#endif

#if EN_clear==1
/*!
  @brief   ʹ��0/��ɫ/�����������������������
*/
static void Adafruit_NeoPixel_clear(void)
{
    memset(adafruit_neopixel.pixels, 0, adafruit_neopixel.numBytes);
}
#endif

#if (EN_gamma8==1) || (EN_gamma32==1)
/*!
@brief   ���ڻ����������ȵ�����8λ٤��У�����ܡ�ʹ��ɫ�������Ӿ��Ͽ���������ȷ��
@param   X�������ȣ�0(��С���/��)��255(���)��
@return  Ȼ����Խ�Gamma����������ȴ��ݸ�����һ��setPixelColor()������
         ��ʹ�ù̶���GammaУ��ָ��2.6�������ƽ�������е�ƽ���������ƺ��Ǻ���ġ�
         �������Ҫ����ϸ�Ŀ��ƣ�����Ҫ�ṩ���Լ���٤��У�����ܡ�
*/
static uint8_t Adafruit_NeoPixel_gamma8(uint8_t x)
{
    return (_NeoPixelGammaTable[x]); // 0-255 in, 0-255 out
}
#endif

#if EN_gamma32==1
/*!
@brief   ������32λ���RGB��WRGB��ɫ��GammaУ��������ʹ��ɫ�������Ӿ��Ͽ���������ȷ��
@param   x  32-bit packed RGB or WRGB color.
@return  Gamma������������ɫ��Ȼ�������setPixelColor()����֮һ�д��ݡ�
         ��Gamma8()һ������ʹ�ù̶���GammaУ��ָ��2.6������ƽ�������е�ƽ��NeoPixels�ƺ������ԡ�
         �������Ҫ����ϸ�Ŀ��ƣ�����Ҫ�ṩ���Լ���٤��У�����ܡ�
*/
static uint32_t Adafruit_NeoPixel_gamma32(uint32_t x)
{
    uint8_t *y = (uint8_t *)&x;
    uint8_t i
    // All four bytes of a 32-bit value are filtered even if RGB (not WRGB),
    // to avoid a bunch of shifting and masking that would be necessary for
    // properly handling different endianisms (and each byte is a fairly
    // trivial operation, so it might not even be wasting cycles vs a check
    // and branch for the RGB case). In theory this might cause trouble *if*
    // someone's storing information in the unused most significant byte
    // of an RGB value, but this seems exceedingly rare and if it's
    // encountered in reality they can mask values going in or coming out.
    for (i = 0; i < 4; i++)
        y[i] = Adafruit_NeoPixel_gamma8(y[i]);
    return x; // Packed 32-bit return
}
#endif

#if EN_rainbow==1
/*!
  @brief    ��һ������ɫ��ѭ�����NeoPixel������
           Everyone loves the rainbow swirl so much, now it's canon!
  @param   first_hue   ��һ�����ص�ɫ����0-65535����ʾɫ�ֵ�һ���������ڡ�
                       ÿ���������ؽ���ƫ�����������ĳ��������һ������ѭ����
  @param   reps        ɫ�������������ϵ�ѭ��������Ĭ��ֵΪ1����ֵ�����ڷ�תɫ��˳��
  @param   saturation  ���Ͷ�(��ѡ)��0-255=��ɫ����ɫ����Ĭ��Ϊ255��
  @param   brightness  Brightness/value (optional), 0-255 = off to max,
                       default = 255. This is distinct and in combination
                       with any configured global strip brightness.
  @param   gammify     If true (default), apply gamma correction to colors
                       for better appearance.
*/
static void Adafruit_NeoPixel_rainbow(uint16_t first_hue, int8_t reps, uint8_t saturation, uint8_t brightness, bool gammify)
{
    uint16_t i;
    for (i=0; i<adafruit_neopixel.numLEDs; i++)
    {
        uint16_t hue = first_hue + (i * reps * 65536) / adafruit_neopixel.numLEDs;
        uint32_t color = Adafruit_NeoPixel_ColorHSV(hue, saturation, brightness);
        if (gammify) color = Adafruit_NeoPixel_gamma32(color);
        Adafruit_NeoPixel_setPixelColor(i, color);
    }
}
#endif

#if EN_numPixels==1
/*!
@brief   ����Adafruit_NeoPixel���������е���������
@return  ���ؼ���(���δ���ã���Ϊ0)��
*/
uint16_t Adafruit_NeoPixel_numPixels(void)
{
    return adafruit_neopixel.numLEDs;
}
#endif

#if EN_getPixels==1
/*!
@brief   ��ȡһ��ֱ��ָ��RAM�е�NeoPixel���ݻ�������ָ�롣
         �����������豸������ʽ�洢(����neo_*����һ��)�����ﲻ����ת����
         ���ʴ˻�������Ӧ�ó�����Ҫ֪���ض������ݸ�ʽ���ʵ��ش�����ɫ��
@return  ָ��NeoPixel������(uint8_t*����)��ָ�롣
@note    �������ڶ�ÿ�����ص���setPixelColor()̫���ĸ�����Ӧ�ó���(����POV��ƹ������Ŀ)��
         ��������û�б߽��飬���д�볬����������ĩ�ˣ�����ܻ���ɾ޴�Ļ��ҡ�
         ǿ���������ΰ������Σ��Լ�������Щ��
*/
uint8_t *Adafruit_NeoPixel_getPixels(void)
{
    return adafruit_neopixel.pixels;
}
#endif

/*!
  @brief   �๹�캯�����ڱ���ʱ��֪���ȡ��ܽź��������͵�NeoPixel���캯����
  @param   n  NeoPixel ������
  @param   t  ��������--��Adafruit_NeoPixel.h�ж����neo_*������ӣ�
              ���磬����NeoPixels��NEO_GRB+NEO_KHZ800��Ҫ800 kHz(Vs 400 KHz)��������
              ����ɫ�ֽ���ÿ���ص���ɫ����ɫ����ɫ˳���ʾ��
  @return  Adafruit_NeoPixel������ʹ��֮ǰ����Begin()������
*/
static void Adafruit_NeoPixel_init(uint16_t n, neoPixelType t)
{
    adafruit_neopixel.begun = false;
    adafruit_neopixel.brightness = 0;
    adafruit_neopixel.pixels = NULL;
    Adafruit_NeoPixel_updateType(t);
    Adafruit_NeoPixel_updateLength(n);
}

/*!
  @brief   �յ��๹�캯����������ʱ��֪�����ȡ��ܽź�/����������ʱ��
           �������Ժ�ʹ��updatType()��updateLength()��setPin()���г�ʼ����
  @return  Adafruit_NeoPixel������ʹ��֮ǰ����Begin()������
  @note    �˺��������ã��˴��������ڿ������ڵ������ľ���Ŀ��
           ����ĿӦ�ø��ô��е�һ�����캯���﷨(length, type)�ġ�new���ؼ��֡�
*/
static void Adafruit_NeoPixel_init_empty(void)
{
    adafruit_neopixel.begun = false;
    adafruit_neopixel.numLEDs = 0;
    adafruit_neopixel.numBytes = 0;
    adafruit_neopixel.brightness = 0;
    adafruit_neopixel.pixels = NULL;
    adafruit_neopixel.rOffset = 1;
    adafruit_neopixel.gOffset = 0;
    adafruit_neopixel.bOffset = 2;
    adafruit_neopixel.wOffset = 1;
}
/*!
  @brief   ������������ȡ������Adafruit_NeoPixel����
*/
static void Adafruit_NeoPixel_deinit(void)
{
    free(adafruit_neopixel.pixels);
}

#if EN_sin8==1
/*!
@brief   8λ�������Ҳ��������뻡�Ȼ�����ȱ�׼���ǵ�λ��ֱ�Ӽ��ݡ�
@param   X����ǣ�0-255��256��ѭ���ص��㣬���Բ(�൱��360�Ȼ�2Բ���ʻ���)��
         ��ˣ�����ʹ���޷���8λ�������򵥵ؽ��мӷ��������
         �Ӷ����������/���磬��������Ȼִ��Ԥ�ڵ�����������
@return  ���ҽ��Ϊ0��255���������ת��Ϊ�з��ŵ�int8_t����Ϊ-128��+127��
         �����ܿ���ϣ���޷��ţ���Ϊ�����ͨ�����ڶ���Ч���е��������ȡ�
*/
static uint8_t Adafruit_NeoPixel_sine8(uint8_t x)
{
    return _NeoPixelSineTable[x]; // 0-255 in, 0-255 out
}
#endif

#if EN_Color_rgb==1
/*!
@brief   �������ĺ졢�̺���ֵת��Ϊ��������ġ�32λRGB��ɫ����
@param   r  Red ����, 0 to 255.
@param   g  Green ����, 0 to 255.
@param   b  Blue ����, 0 to 255.
@return  32λ�����RGBֵ��Ȼ����Խ��丳�������Թ��Ժ�ʹ�û򴫵ݸ�setPixelColor()������
         ѹ��RGB��ʽ�ǿ�Ԥ��ģ���LED������ɫ˳���޹ء�
*/
static uint32_t Adafruit_NeoPixel_Color_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
#endif

#if EN_Color_rgbw==1
/*!
@brief   ���졢�̡��������ĸ�������ֵת��Ϊһ�������ġ������32λWRGB��ɫ��
@param   r  Red ����, 0 to 255.
@param   g  Green ����, 0 to 255.
@param   b  Blue ����, 0 to 255.
@param   w  White ����, 0 to 255.
@return  32λ�����WRGBֵ��Ȼ����Խ��丳�������Թ��Ժ�ʹ�û򴫵ݸ�setPixelColor()������
         �����WRGB��ʽ�ǿ�Ԥ��ģ�����LED��������ɫ˳����Ρ�
*/
static uint32_t Adafruit_NeoPixel_Color_rgbw(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
    return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
#endif

Tst_Adafruit_NeoPixel  adafruit_neopixel=
{
    // ���ܺ���
    Adafruit_NeoPixel_begin,
    Adafruit_NeoPixel_updateLength,
    Adafruit_NeoPixel_updateType,
    Adafruit_NeoPixel_show,

    #if EN_setPixelColor_rgb==1
    Adafruit_NeoPixel_setPixelColor_rgb,
    #else
    NULL,
    #endif

    #if EN_setPixelColor_rgbw==1
    Adafruit_NeoPixel_setPixelColor_rgbw,
    #else
    NULL,
    #endif

    #if EN_setPixelColor==1
    Adafruit_NeoPixel_setPixelColor,
    #else
    NULL,
    #endif

    #if EN_fill==1
    Adafruit_NeoPixel_fill,
    #else
    NULL,
    #endif

    #if EN_ColorHSV==1
    Adafruit_NeoPixel_ColorHSV,
    #else
    NULL,
    #endif

    #if EN_getPixelColor==1
    Adafruit_NeoPixel_getPixelColor,
    #else
    NULL,
    #endif

    Adafruit_NeoPixel_setBrightness,

    #if EN_getBrightness==1
    Adafruit_NeoPixel_getBrightness,
    #else
    NULL,
    #endif

    #if EN_clear==1
    Adafruit_NeoPixel_clear,
    #else
    NULL,
    #endif

    #if EN_rainbow==1
    Adafruit_NeoPixel_rainbow,
    #else
    NULL,
    #endif

    #if EN_numPixels==1
    Adafruit_NeoPixel_numPixels,
    #else
    NULL,
    #endif

    #if EN_getPixels==1
    Adafruit_NeoPixel_getPixels,
    #else
    NULL,
    #endif

    #if EN_Color_rgb==1
    Adafruit_NeoPixel_Color_rgb,
    #else
    NULL,
    #endif

    #if EN_Color_rgbw==1
    Adafruit_NeoPixel_Color_rgbw,
    #else
    NULL,
    #endif

    Adafruit_NeoPixel_init,
    Adafruit_NeoPixel_init_empty,
    Adafruit_NeoPixel_deinit,
    #if (EN_gamma8==1) || (EN_gamma32==1)
    Adafruit_NeoPixel_gamma8,
    #else
    NULL,
    #endif
    
    #if EN_gamma32==1
    Adafruit_NeoPixel_gamma32,
    #else
    NULL,
    #endif
    
    #if EN_sin8==1
    Adafruit_NeoPixel_sine8,
    #else
    NULL,
    #endif
    
    // Ӳ���ӿ�
    NULL,

    // ����
    false,
    0,
    0,
    0,
    NULL,
    0,
    0,
    0,
    0
};

#undef EN_setPixelColor_rgb
#undef EN_setPixelColor_rgbw
#undef EN_setPixelColor
#undef EN_fill
#undef EN_ColorHSV
#undef EN_getPixelColor
#undef EN_getBrightness
#undef EN_clear
#undef EN_rainbow
#undef EN_numPixels
#undef EN_getPixels
#undef EN_Color_rgb
#undef EN_Color_rgbw
#undef EN_gamma8
#undef EN_gamma32
#undef EN_sin8
