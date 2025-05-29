cmake_minimum_required(VERSION 3.19)

# Check [pico-sdk] and [picotool]
include(cmake/bringup_helper.cmake)

# Include the SDK import
include(pico_sdk_import.cmake)

# Project name
project(RP2040_LCD_1.28_Graphics)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Collect all font sources
file(GLOB FONT_SOURCES
        ${CMAKE_SOURCE_DIR}/src/fonts/*.cpp
)

# Add executables
add_executable(main
        app/main.c
        src/platform/platform.c
        ${FONT_SOURCES}
)

# Include source files directories
target_include_directories(main PRIVATE
        ${CMAKE_SOURCE_DIR}/src/platform
        ${CMAKE_SOURCE_DIR}/src/lcd
        ${CMAKE_SOURCE_DIR}/src/fonts
)

# Add all necessary libraries
target_link_libraries(main
        PRIVATE
        pico_stdlib
        hardware_pwm
        hardware_spi
        hardware_dma
)

# Create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(main)

# Change uart pinout for main target TODO: create platform remap
target_compile_definitions(main
        PRIVATE
        PICO_DEFAULT_UART_TX_PIN=16
        PICO_DEFAULT_UART_RX_PIN=17
)