#include <stdio.h>
#include "pico/stdlib.h"
#include "platform.h"

int main() {
    platform_init();
    platform_set_backlight_level(30);
    printf("Hello, LCD!\n");
    return 0;
}