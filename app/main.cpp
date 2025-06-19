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
    lcd.drawText(70, 210, "SPS TECH", &permanentmarker_regular_12, LCD_DARKGREY, LCD_BLACK);

    lcd.drawCircle(168, 60, 29, RGB565(149,235,172), 1, true);
    lcd.drawText(150, 41, "17", &oswald_bold_20, LCD_BLACK, RGB565(149,228,172));
    lcd.drawText(145, 65, "JUNE", &oswald_bold_12, LCD_BLACK, RGB565(149,228,172));

    //Time (just demo)
    lcd.drawText(72,  100, ":",  &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(142, 100, ":",  &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(20,  100, "15", &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(90,  100, "01", &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(160, 100, "38", &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);

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
        QMI8658_read_xyz(acc, gyro, &tim_count);
        lcd.drawText(50,  155, &oswald_medium_12, LCD_DARKGREY, LCD_BLACK, "X     Y     Z");
        lcd.drawText(30,  173, &oswald_light_12,  LCD_DARKGREY, LCD_BLACK, "%.1f", gyro[0]);
        lcd.drawText(95,  173, &oswald_light_12,  LCD_DARKGREY, LCD_BLACK, "%.1f", gyro[1]);
        lcd.drawText(170, 173, &oswald_light_12,  LCD_DARKGREY, LCD_BLACK, "%.2f", gyro[2]);
        lcd.update();
    }

    return 0;
}