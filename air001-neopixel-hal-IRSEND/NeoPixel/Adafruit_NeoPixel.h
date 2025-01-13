#ifndef ADAFRUIT_NEOPIXEL_H
#define ADAFRUIT_NEOPIXEL_H

#include <stdbool.h>
#include <stdint.h>

// NeoPixel�������е�ԭɫ˳����ܻ����豸���͡�����������ͬһ��Ʒ�Ĳ�ͬ�汾��������ͬ��
// Adafruit_NeoPixel���캯���ĵ��������������������к�ɫ����ɫ����ɫ��ɫ(������ڣ���������ɫ)��
// ÿ�����ֽ�ƫ����--����#defineΪÿ�������ṩ��һ��������ʹ�õ������汾��
// ���磬NEO_GRB��ʾ��NeoPixel���ݵ��豸����ÿ�����������ֽڣ�����ĵ�һ���ֽڰ�����ɫֵ���ڶ���������ɫ��������������ɫ��
// NeoPixels�����ڴ��ʾ��������˳����ͬ��������������ʱ����Ҫ���������ֽڡ�
// ��Щֵ�еĴ���������������ʵ������豸�У������������ַ�ʽ��ɵģ�
// ���������Ѿ�������׼��(���⣬���ʹ��WS2811��������IC�����ܻ����κ���ֵ������������ǵ�����)��

// λ5��4�Ǵ����صĵ�һ���ֽڵ���ɫ�ֽ�λ�õ�ƫ����(0-3)��
// λ3��2����ɫƫ������1��0����ɫƫ������
// �����RGBW������(��R��G��B�⻹֧�ְ�ɫԭɫ)����λ7��6Ϊ���ֽڵ�ƫ������
// ����λ7��6������Ϊ��5��4(��ɫ)��ͬ��ֵ����ָʾRGB(��RGBW)������
// �������Ʊ�ʾ����
// 0bWWRRGGBB for RGBW devices
// 0bRRRRGGBB for RGB

// RGB NeoPixel ����; ��ɫ�ͺ�ɫ��ƫ����ʼ����ͬ
// Offset:        W          R          G          B
#define NEO_RGB ((0 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B
#define NEO_RBG ((0 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G
#define NEO_GRB ((1 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B
#define NEO_GBR ((2 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R
#define NEO_BRG ((1 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G
#define NEO_BGR ((2 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R

// RGBW NeoPixel ����; ����4��ƫ�������ǲ�ͬ��
// Offset:         W          R          G          B
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3)) ///< Transmit as W,R,G,B
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2)) ///< Transmit as W,R,B,G
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3)) ///< Transmit as W,G,R,B
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2)) ///< Transmit as W,G,B,R
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1)) ///< Transmit as W,B,R,G
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1)) ///< Transmit as W,B,G,R

#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3)) ///< Transmit as R,W,G,B
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2)) ///< Transmit as R,W,B,G
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3)) ///< Transmit as R,G,W,B
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2)) ///< Transmit as R,G,B,W
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1)) ///< Transmit as R,B,W,G
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1)) ///< Transmit as R,B,G,W

#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3)) ///< Transmit as G,W,R,B
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2)) ///< Transmit as G,W,B,R
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3)) ///< Transmit as G,R,W,B
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2)) ///< Transmit as G,R,B,W
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,W,R
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1)) ///< Transmit as G,B,R,W

#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0)) ///< Transmit as B,W,R,G
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0)) ///< Transmit as B,W,G,R
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0)) ///< Transmit as B,R,W,G
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0)) ///< Transmit as B,R,G,W
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,W,R
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0)) ///< Transmit as B,G,R,W

// ��֧��400KHzͨ�����ʵ��豸
#define NEO_KHZ800 0x0000 ///< 800 KHz data transmission

typedef uint8_t neoPixelType; ///< 3rd arg to Adafruit_NeoPixel constructor

/*!
    @brief  Adafruit NeoPixels���ܺ���ʹ�ܿ���
*/
#define EN_setPixelColor_rgb    0
#define EN_setPixelColor_rgbw   0
#define EN_setPixelColor        1
#define EN_fill                 0
#define EN_ColorHSV             1
#define EN_getPixelColor        0
#define EN_getBrightness        0
#define EN_clear                0
#define EN_rainbow              0
#define EN_numPixels            1
#define EN_getPixels            0
#define EN_Color_rgb            1
#define EN_Color_rgbw           0
/*!
    @brief  Adafruit NeoPixels�ڲ����ܺ���ʹ�ܿ���
*/
#define EN_gamma8               0//1
#define EN_gamma32              0
#define EN_sin8                 0//1

/*!
    @brief  ���Ͷ���洢��Adafruit NeoPixels�ͼ����豸������״̬�ͺ����Ľṹ��
*/
typedef struct
{
    // ���ܺ���
    void (*begin)(void);// �������������NeoPixel�ܽ�
    void (*updateLength)(uint16_t);
    void (*updateType)(neoPixelType);
    void (*show)(void);
    void (*setPixelColor_rgb)(uint16_t, uint8_t, uint8_t, uint8_t);
    void (*setPixelColor_rgbw)(uint16_t, uint8_t, uint8_t, uint8_t, uint8_t);
    void (*setPixelColor)(uint16_t, uint32_t);
    void (*fill)(uint32_t, uint16_t, uint16_t);
    uint32_t (*ColorHSV)(uint16_t, uint8_t, uint8_t);
    uint32_t (*getPixelColor)(uint16_t);
    void (*setBrightness)(uint8_t);
    uint8_t (*getBrightness)(void);
    void (*clear)(void);
    void (*rainbow)(uint16_t, int8_t, uint8_t, uint8_t, bool);
    uint16_t (*numPixels)(void);
    uint8_t *(*getPixels)(void);
    uint32_t (*Color_rgb)(uint8_t, uint8_t, uint8_t);
    uint32_t (*Color_rgbw)(uint8_t, uint8_t, uint8_t, uint8_t);
    void (*init)(uint16_t, neoPixelType);
    void (*init_empty)(void);
    void (*deinit)(void);
    uint8_t (*gamma8)(uint8_t);
    uint32_t (*gamma32)(uint32_t);
    uint8_t (*sine8)(uint8_t);
    
    // Ӳ���ӿ�
    void (*NeoPixel_op)(uint8_t *,uint16_t);
    
    // ����
    bool begun;         ///< �����ǰ������Begin()����ΪTrue
    uint16_t numLEDs;   ///< �����е�RGB LED����
    uint16_t numBytes;  ///< ���桮Pixels���������Ĵ�С
    uint8_t brightness; ///< ��������0-255(�洢Ϊ+1)
    uint8_t *pixels;    ///< ����LED��ɫֵ(ÿ��ֵ3��4���ֽ�)
    uint8_t rOffset;    ///< ÿ��3�ֽڻ�4�ֽ������ڵĺ�ɫ�ֽڵ�����
    uint8_t gOffset;    ///< ��ɫ�ֽڵ�����
    uint8_t bOffset;    ///< ��ɫ�ֽڵ�����
    uint8_t wOffset;    ///< ��ɫ�ֽڵ����� (���û�а�ɫ�������rOffset)
}Tst_Adafruit_NeoPixel;

extern Tst_Adafruit_NeoPixel adafruit_neopixel;
#endif // ADAFRUIT_NEOPIXEL_H
