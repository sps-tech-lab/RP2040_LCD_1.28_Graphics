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
    lcd.drawText(70, 210, "SPS TECH", &permanentmarker_regular_12, LCD_DARKGREY, LCD_BLACK);

    lcd.drawCircle(170, 60, 29, RGB565(149,228,172), 1, true);
    lcd.drawText(151, 41, "17", &oswald_medium_20, LCD_BLACK, RGB565(149,228,172));
    lcd.drawText(148, 65, "JUNE", &oswald_medium_12, LCD_BLACK, RGB565(149,228,172));

    //Time
    lcd.drawText(72,  100, ":",  &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(142, 100, ":",  &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(20,  100, "15", &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(90,  100, "01", &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);
    lcd.drawText(160, 100, "38", &oswald_medium_36, LCD_DARKGREY, LCD_BLACK);

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
    // while (1)
    // {
    //     QMI8658_read_xyz(acc, gyro, &tim_count);
    //     lcd.drawText(68, 30, &oswald_medium_12, LCD_DARKGREY, LCD_BLACK, "G.x %.2f", gyro[0]);
    //     lcd.drawText(68, 50, &oswald_medium_12, LCD_DARKGREY, LCD_BLACK, "G.y %.2f", gyro[1]);
    //     lcd.drawText(68, 70, &oswald_medium_12, LCD_DARKGREY, LCD_BLACK, "G.z %.2f", gyro[2]);
    //
    //     lcd.drawText(85, 180, &oswald_medium_36, LCD_DARKGREY, LCD_BLACK, "%d", counter);
    //     lcd.update();
    //     if(++counter > 99){
    //         counter = 0;
    //     }
    //
    // }

    return 0;
}