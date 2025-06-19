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

//Default fonts
extern font font24;
extern font font20;
extern font font16;
extern font font12;

//Roboto fonts
extern font roboto_reg_12;
extern font roboto_reg_16;
extern font roboto_reg_20;
extern font roboto_reg_24;

//Fancy fonts
extern font permanentmarker_regular_12;
extern font permanentmarker_regular_20;

//Oswald fonts
extern font oswald_light_12;
extern font oswald_light_16;
extern font oswald_light_20;
extern font oswald_light_24;
extern font oswald_light_36;

extern font oswald_medium_12;
extern font oswald_medium_16;
extern font oswald_medium_20;
extern font oswald_medium_24;
extern font oswald_medium_36;

extern font oswald_regular_12;
extern font oswald_regular_16;
extern font oswald_regular_20;
extern font oswald_regular_24;
extern font oswald_regular_36;


#endif
