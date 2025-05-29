//
// Created by SPS on 28/05/2025.
//
#ifndef PLATFORM_H
#define PLATFORM_H

#include <stdbool.h>
#include <stdint.h>


bool platform_init(void);
void platform_set_backlight_level(uint8_t level);

#endif //PLATFORM_H
