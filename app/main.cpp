#include <stdio.h>
#include "pico/stdlib.h"
#include "platform.hpp"

#include "lcd.hpp"
#include "fonts.hpp"

//Test framebuffer
static uint16_t fbuf[240*240];


int main() {
    platform_init();
    printf("Hello, LCD!\n");

    platform_set_backlight_level(30);

    LCD lcd((uint16_t *) &fbuf, HORIZONTAL);

    lcd.fillScreen(LCD_BLACK);
    lcd.drawText(68, 108, "RP2040 TEST", &font12, LCD_DARKGREY, LCD_BLACK);

    //Color test
    lcd.drawRect( 20, 140, 40, 160,     LCD_MAROON,     1, true);
    lcd.drawRect( 40, 140, 60, 160,     LCD_RED,        1, true);
    lcd.drawRect( 60, 140, 80, 160,     LCD_ORANGE,     1, true);
    lcd.drawRect( 80, 140, 100, 160,    LCD_YELLOW,     1, true);
    lcd.drawRect( 100, 140, 120, 160,   LCD_GREENYELLOW,1, true);
    lcd.drawRect( 120, 140, 140, 160,   LCD_GREEN,      1, true);
    lcd.drawRect( 140, 140, 160, 160,   LCD_CYAN,       1, true);
    lcd.drawRect( 160, 140, 180, 160,   LCD_BLUE,       1, true);
    lcd.drawRect( 180, 140, 200, 160,   LCD_MAGENTA,    1, true);
    lcd.drawRect( 200, 140, 220, 160,   LCD_PURPLE,     1, true);

    lcd.update();

    uint8_t counter = 0;
    while (1)
    {
        lcd.drawText(100, 180, &font24, LCD_DARKGREY, LCD_BLACK, "%d", counter);
        lcd.update();
        if(++counter > 99){
            counter = 0;
        }
    }

    return 0;
}