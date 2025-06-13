# Example of extension board specifics #

#Redefines the default UART TX and RX pins for the "waveshare_lcd_1.28_pin_head" extension board
target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        PICO_DEFAULT_UART_TX_PIN=16
        PICO_DEFAULT_UART_RX_PIN=17
)

