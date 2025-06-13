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
add_executable(${PROJECT_NAME}
        app/main.cpp
        src/platform/platform.cpp
        src/lcd/framebuffer.cpp
        src/lcd/lcd.cpp
        ${FONT_SOURCES}
)

# Include source files directories
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src/platform
        ${CMAKE_SOURCE_DIR}/src/lcd
        ${CMAKE_SOURCE_DIR}/src/fonts
)

# Add all necessary libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        pico_stdlib
        hardware_pwm
        hardware_spi
        hardware_dma
)

# Create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${PROJECT_NAME})

# Add ELF size table to build report
include(cmake/pico_size_tool.cmake)

# Add extension board (if it specified)
if(DEFINED EXT_BOARD AND NOT EXT_BOARD STREQUAL "")
    message(STATUS "Using extension board [${EXT_BOARD}] specifics")
    include(ext_boards/${EXT_BOARD}/ext_board.cmake)
endif()


