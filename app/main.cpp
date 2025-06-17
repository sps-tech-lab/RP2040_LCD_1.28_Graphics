#include <stdio.h>
#include "pico/stdlib.h"
#include "platform.hpp"

#include "lcd.hpp"
#include "fonts.hpp"
#include "QMI8658.h"

//Test framebuffer
static uint16_t fbuf[240*240];


int main() {
    stdio_init_all();

    printf("\nHello, LCD!\n");

    platform_init();

    platform_set_backlight_level(30);

    LCD lcd((uint16_t *) &fbuf, HORIZONTAL);

    lcd.fillScreen(LCD_BLACK);
    lcd.drawText(28, 100, "Rp2040 Test", &PermanentMarker_Regular_20, LCD_DARKGREY, LCD_BLACK);

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

    //IMU test
    float acc[3], gyro[3];
    unsigned int tim_count = 0;

    QMI8658_init();
    QMI8658_read_xyz(acc, gyro, &tim_count);
    printf("acc_x   = %4.3fmg , acc_y  = %4.3fmg , acc_z  = %4.3fmg\r\n", acc[0], acc[1], acc[2]);
    printf("gyro_x  = %4.3fdps, gyro_y = %4.3fdps, gyro_z = %4.3fdps\r\n", gyro[0], gyro[1], gyro[2]);
    printf("tim_count = %d\r\n", tim_count);

    uint8_t counter = 0;
    while (1)
    {
        QMI8658_read_xyz(acc, gyro, &tim_count);
        lcd.drawText(68, 30, &roboto_reg_12, LCD_DARKGREY, LCD_BLACK, "G.x %.2f", gyro[0]);
        lcd.drawText(68, 50, &roboto_reg_12, LCD_DARKGREY, LCD_BLACK, "G.y %.2f", gyro[1]);
        lcd.drawText(68, 70, &roboto_reg_12, LCD_DARKGREY, LCD_BLACK, "G.z %.2f", gyro[2]);

        lcd.drawText(100, 180, &roboto_reg_24, LCD_DARKGREY, LCD_BLACK, "%d", counter);
        lcd.update();
        if(++counter > 99){
            counter = 0;
        }

    }

    return 0;
}