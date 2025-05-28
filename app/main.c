#include <stdio.h>
#include "pico/stdlib.h"
#include "platform.h"

int main() {
    platform_init();
    printf("Hello, LCD!\n");
    return 0;
}