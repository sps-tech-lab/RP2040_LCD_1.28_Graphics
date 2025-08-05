#include <stdio.h>
#include "pico/stdlib.h"
#include "platform.hpp"

#include "lcd.hpp"
#include "fonts.hpp"
#include "bitmaps.hpp"
#include "QMI8658.h"

//Test framebuffer
static uint16_t fbuf[240*240];

//--------------------------------------------------------------------------
//RED POINT TEST
//--------------------------------------------------------------------------
// Constants
constexpr int SCREEN_WIDTH = 240;
constexpr int SCREEN_HEIGHT = 240;
constexpr int CENTER_X = SCREEN_WIDTH / 2;
constexpr int CENTER_Y = SCREEN_HEIGHT / 2;
constexpr int RADIUS = 5;

static float filtered_acc[3] = {0};

// Limit movement to prevent circle from leaving screen
int clamp(int val, int min_val, int max_val) {
    return (val < min_val) ? min_val : (val > max_val) ? max_val : val;
}

// Main draw function using accelerometer values
void drawTiltCircleFromAccel(const float acc[3], LCD& lcd) {

    // Normalize acceleration to ±1g range
    float normX = acc[1] / 1000.0f;  // Tilt left/right
    float normY = acc[0] / 1000.0f;  // Tilt forward/backward

    // Clamp to reasonable tilt range
    if (normX > 1.0f) normX = 1.0f;
    if (normX < -1.0f) normX = -1.0f;
    if (normY > 1.0f) normY = 1.0f;
    if (normY < -1.0f) normY = -1.0f;

    // Map tilt to pixel shifts from center
    int maxShiftX = CENTER_X - RADIUS;
    int maxShiftY = CENTER_Y - RADIUS;
    int circleX = clamp(CENTER_X + static_cast<int>(normX * maxShiftX), RADIUS, SCREEN_WIDTH - RADIUS);
    int circleY = clamp(CENTER_Y - static_cast<int>(normY * maxShiftY), RADIUS, SCREEN_HEIGHT - RADIUS);  // minus for screen Y direction

    // Draw RedPoint
    lcd.drawCircle(circleX, circleY, RADIUS, LCD_RED, 1, true);
}


//--------------------------------------------------------------------------


int main() {
    stdio_init_all();

    printf("\nHello, LCD!\n");

    platform_init();

    platform_set_backlight_level(30);

    LCD lcd((uint16_t *) &fbuf, HORIZONTAL);
    lcd.fillScreen(LCD_BLACK);

    // IMU test
    float acc[3], gyro[3];
    unsigned int tim_count = 0;

    QMI8658_init();
    QMI8658_read_xyz(acc, gyro, &tim_count);
    // printf("acc_x   = %4.3fmg , acc_y  = %4.3fmg , acc_z  = %4.3fmg\r\n", acc[0], acc[1], acc[2]);
    // printf("gyro_x  = %4.3fdps, gyro_y = %4.3fdps, gyro_z = %4.3fdps\r\n", gyro[0], gyro[1], gyro[2]);
    // printf("tim_count = %d\r\n", tim_count);


    while (1)
    {
        // IMU runtime test
        QMI8658_read_xyz(acc, gyro, &tim_count);

        // Smoothing
        // (slow <- 0.0 ... 1.0 -> fast)
        float alpha = 0.01f;
        for (int i = 0; i < 3; ++i) {
            filtered_acc[i] = alpha * acc[i] + (1.0f - alpha) * filtered_acc[i];
        }

        // Draw RedPiont
        lcd.fillScreen(LCD_BLACK);
        drawTiltCircleFromAccel(acc, lcd);
        lcd.update();

        printf("acc_x   = %4.3fmg , acc_y  = %4.3fmg , acc_z  = %4.3fmg\r\n", acc[0], acc[1], acc[2]);
    }
    return 0;
}