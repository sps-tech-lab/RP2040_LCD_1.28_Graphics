#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

//Includes
#include "fonts.hpp"


// Predefined colors
#define LCD_BLACK       0x0000
#define LCD_WHITE       0xFFFF
#define LCD_DARKGREY    0x7BEF
#define LCD_LIGHTGREY   0xC618
#define LCD_RED         0xF800
#define LCD_ORANGE      0xFD20
#define LCD_YELLOW      0xFFE0
#define LCD_GREENYELLOW 0xAFE5
#define LCD_GREEN       0x07E0
#define LCD_CYAN        0x07FF
#define LCD_BLUE        0x001F
#define LCD_PINK        0xFC18
#define LCD_MAGENTA     0xF81F
#define LCD_PURPLE      0x780F
#define LCD_MAROON      0x7800
#define LCD_DARKGREEN   0x03E0
#define LCD_OLIVE       0x7BE0
#define LCD_DARKCYAN    0x03EF
#define LCD_NAVY        0x000F

/**
 * @brief Convert 8-bit R,G,B (0–255) into a 16-bit RGB565 value.
 */
#define RGB565(r, g, b) ( \
(uint16_t)( ((uint16_t)((r) & 0xF8) << 8) | \
((uint16_t)((g) & 0xFC) << 3) | \
((uint16_t)((b) & 0xF8) >> 3) ) )

struct FrameBuffer
{
    uint16_t* canvas;
    uint8_t width;
    uint8_t height;

    FrameBuffer(uint16_t*, uint8_t, uint8_t);

    // draw
    virtual void update() = 0;
    void fillScreen(uint16_t);
    void setPixel(uint8_t, uint8_t, uint16_t);
    void drawPoint(uint8_t, uint8_t, uint16_t, uint8_t);
    void drawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint8_t);
    void drawHorizontalLine(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t);
    void drawVerticalLine(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t);
    void drawRect(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint8_t, bool);
    void drawCircle(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t, bool);

    uint16_t alphaBlend(uint8_t, uint16_t, uint16_t);
    void darwGradientRect(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t, uint16_t, bool);

    // text
    void drawChar(uint8_t, uint8_t, const char, font*, uint16_t, uint16_t);
    void drawText(uint8_t, uint8_t, const char *, font*, uint16_t, uint16_t);
    void drawText(uint8_t, uint8_t, font*, uint16_t, uint16_t, const char* fmt, ...);

    /**
     * @brief Draw a full-color bitmap.
     * @param x      top-left x
     * @param y      top-left y
     * @param w      bitmap width in pixels
     * @param h      bitmap height in pixels
     * @param data   pointer to w*h uint16_t pixels (row-major)
     */
    void drawBitmap(uint8_t x, uint8_t y,
                    uint8_t w, uint8_t h,
                    const uint16_t* data);

    /**
     * @brief Draw a packed 1-bit monochrome bitmap.
     * @param x      top-left x
     * @param y      top-left y
     * @param w      bitmap width in pixels
     * @param h      bitmap height in pixels
     * @param data   pointer to ( (w+7)/8 * h ) bytes, MSB first
     * @param fg     pixel color when bit==1
     * @param bg     pixel color when bit==0
     */
    void drawMonoBitmap(uint8_t x, uint8_t y,
                        uint8_t w, uint8_t h,
                        const uint8_t* data,
                        uint16_t fg, uint16_t bg);
};

#endif //FRAMEBUFFER_H





