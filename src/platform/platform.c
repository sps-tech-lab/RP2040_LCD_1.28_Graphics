//
// Created by SPS on 28/05/2025.
//
#include "platform.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

//Local data
static struct
{
    uint    slice;
    int     dma_channel;
}platform;


bool platform_init(void)
{
    stdio_init_all();

    //LCD config
    gpio_init(WAVESHARE_LCD_DC_PIN);
    gpio_init(WAVESHARE_LCD_CS_PIN);
    gpio_init(WAVESHARE_LCD_SCLK_PIN);
    gpio_init(WAVESHARE_LCD_TX_PIN);
    gpio_init(WAVESHARE_LCD_RST_PIN);
    gpio_init(WAVESHARE_LCD_BL_PIN);

    gpio_set_dir(WAVESHARE_LCD_DC_PIN,GPIO_OUT);
    gpio_set_dir(WAVESHARE_LCD_CS_PIN,GPIO_OUT);
    gpio_set_dir(WAVESHARE_LCD_SCLK_PIN,GPIO_OUT);
    gpio_set_dir(WAVESHARE_LCD_TX_PIN,GPIO_OUT);
    gpio_set_dir(WAVESHARE_LCD_RST_PIN,GPIO_OUT);
    gpio_set_dir(WAVESHARE_LCD_BL_PIN,GPIO_OUT);

    gpio_put(WAVESHARE_LCD_CS_PIN, true);
    gpio_put(WAVESHARE_LCD_DC_PIN, false);
    gpio_put(WAVESHARE_LCD_BL_PIN, true);

    //PWM Config
    gpio_set_function(WAVESHARE_LCD_BL_PIN, GPIO_FUNC_PWM);

    platform.slice = pwm_gpio_to_slice_num(WAVESHARE_LCD_BL_PIN);

    pwm_set_wrap(platform.slice, 100);
    pwm_set_chan_level(platform.slice, PWM_CHAN_B, 0);
    pwm_set_clkdiv(platform.slice, 50);
    pwm_set_enabled(platform.slice, true);

    // SPI Config
    spi_init(SPI_INSTANCE(WAVESHARE_LCD_SPI), 62500000); //Set maximal 62.5 MHz
    gpio_set_function(WAVESHARE_LCD_SCLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(WAVESHARE_LCD_TX_PIN, GPIO_FUNC_SPI);

    //DMA config
    platform.dma_channel = dma_claim_unused_channel(true);

    // Configure the DMA channel for SPI1 TX
    dma_channel_config config = dma_channel_get_default_config(platform.dma_channel);
    channel_config_set_transfer_data_size(&config, DMA_SIZE_8);  // 8-bit transfers
    channel_config_set_dreq(&config, DREQ_SPI1_TX);              // SPI1 TX as DMA request trigger

    dma_channel_configure(
            platform.dma_channel,     // DMA channel
            &config,                  // Channel configuration
            &spi_get_hw(spi1)->dr,    // Write to SPI1's data register
            NULL,                     // Source address (set dynamically in dma_spi_write)
            0,                        // Number of transfers (set dynamically in dma_spi_write)
            false                     // Don't start yet
    );

    printf("Initialization finished\r\n");
    return true;
}

void platform_set_backlight_level(uint8_t level)
{
    if(( level < 0 )||( level > 100 )){
        printf("[Error] wrong backlight_level\r\n");
    }else{
        pwm_set_chan_level(platform.slice, PWM_CHAN_B, level);
    }
}
