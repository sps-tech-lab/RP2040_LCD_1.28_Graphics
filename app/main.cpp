#include <stdio.h>
#include "pico/stdlib.h"
#include "platform.hpp"

#include "lcd.hpp"
#include "fonts.hpp"
#include "bitmaps.hpp"
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
//    lcd.drawLine(LCD_HEIGHT/2, 0, LCD_HEIGHT/2, 240, LCD_RED, 1); //Horisontal ruler
//    lcd.drawLine(0,  LCD_WIDTH/2, 240, LCD_WIDTH/2, LCD_RED, 1);  //Vertical ruler
    lcd.drawText(87, 210, "SPS TECH", &permanentmarker_regular_12, LCD_DARKGREY, LCD_BLACK, 0);

    lcd.drawCircle(168, 60, 29, RGB565(149,235,172), 1, true);
    lcd.drawText(161, 41, "17", &oswald_bold_20, LCD_BLACK, RGB565(149,228,172), 2);
    lcd.drawText(157, 65, "JUNE", &oswald_bold_12, LCD_BLACK, RGB565(149,228,172), 0);

    //Time (just demo)
    lcd.drawText(48,  95, "15:01:38",  &oswald_bold_48, LCD_DARKGREY, LCD_BLACK, 2);

    //gImage
    lcd.draw_gImage( 42,  31, gImage_ava);
    lcd.update();

    //IMU test
    float acc[3], gyro[3];
    unsigned int tim_count = 0;

    QMI8658_init();
    QMI8658_read_xyz(acc, gyro, &tim_count);
    printf("acc_x   = %4.3fmg , acc_y  = %4.3fmg , acc_z  = %4.3fmg\r\n", acc[0], acc[1], acc[2]);
    printf("gyro_x  = %4.3fdps, gyro_y = %4.3fdps, gyro_z = %4.3fdps\r\n", gyro[0], gyro[1], gyro[2]);
    printf("tim_count = %d\r\n", tim_count);

    while (1)
    {
        //IMU runtime test
        //QMI8658_read_xyz(acc, gyro, &tim_count);
        lcd.drawText(91,  155, "X     Y     Z", &oswald_medium_12, LCD_DARKGREY, LCD_BLACK);
        lcd.drawRect(10, 173, 230, 185, LCD_BLACK, 1, true );
        lcd.drawText(32,  173, &oswald_light_12,  LCD_DARKGREY, LCD_BLACK, 2, "%.2f", gyro[0]);
        lcd.drawText(97,  173, &oswald_light_12,  LCD_DARKGREY, LCD_BLACK, 2, "%.2f", gyro[1]);
        lcd.drawText(172, 173, &oswald_light_12,  LCD_DARKGREY, LCD_BLACK, 2, "%.2f", gyro[2]);
        lcd.update();
    }

    return 0;
}