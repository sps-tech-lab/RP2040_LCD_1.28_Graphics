#ifndef __FONTS_H
#define __FONTS_H

#include <stdint.h>

struct font
{
    const uint8_t *table;
    uint8_t width;
    uint8_t height;

    font(const uint8_t* _table, uint8_t _width, uint8_t _height): table(_table), width(_width), height(_height){};
};

extern font font24;
extern font font20;
extern font font16;
extern font font12;

extern font roboto_reg_12;
extern font roboto_reg_16;
extern font roboto_reg_20;
extern font roboto_reg_24;

#endif
