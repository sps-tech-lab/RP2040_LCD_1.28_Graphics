//
// Created by SPS on 28/05/2025.
//
#include "platform.h"
#include "pico/stdlib.h"

bool platform_init(void)
{
    stdio_init_all();
    return true;
}