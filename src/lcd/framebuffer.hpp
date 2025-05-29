/**
 * @author  SPS
 * @date    29/05/2025
 * @brief   Header-only frame buffer implementation for RP2040/RP3050
 */
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

//Includes
#include <cstdint>
#include <stdlib.h>
#include <cstring>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "fonts.hpp"

/**
 * @brief Utility: swap bytes for RGB565/BGR565 toggling
 * @param value 16-bit value to swap
 * @return Byte-swapped 16-bit value
 */
static inline uint16_t swap_bytes(uint16_t value) {
    return static_cast<uint16_t>((value << 8) | (value >> 8));
}

/**
 * @brief Generic frame buffer for RP2040/RP3050 with compile-time dimensions
 * @tparam WIDTH  Buffer width in pixels
 * @tparam HEIGHT Buffer height in pixels
 */
template <uint16_t WIDTH, uint16_t HEIGHT>
class Framebuf {
public:
    static constexpr uint16_t width  = WIDTH;
    static constexpr uint16_t height = HEIGHT;

    /**
     * @brief DMA-friendly pixel buffer (RGB565), 4-byte aligned
     */
    alignas(4) uint16_t buffer[width * height];

    /**
     * @brief Construct a new Framebuf, clearing it to black
     */
    Framebuf() {
        clear(0);
    }

    /**
     * @brief Clear entire buffer to a color
     * @param color 16-bit color in RGB565 format
     */
    inline void clear(uint16_t color) noexcept {
        uint16_t c = swap_bytes(color);
        size_t total = width * height;
        for (size_t i = 0; i < total; ++i) {
            buffer[i] = c;
        }
    }

    /**
     * @brief Set a single pixel (bounds-checked)
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param color 16-bit color in RGB565 format
     */
    inline void pixel(uint16_t x, uint16_t y, uint16_t color) noexcept {
        if (x >= width || y >= height) return;
        buffer[y * width + x] = swap_bytes(color);
    }

    /**
     * @brief Draw a square "point" of given size
     * @param x X-coordinate
     * @param y Y-coordinate
     * @param color 16-bit color in RGB565 format
     * @param size Size of the square in pixels (default 1)
     */
    inline void point(uint16_t x, uint16_t y, uint16_t color, uint16_t size = 1) noexcept {
        for (uint16_t dx = 0; dx < size; ++dx) {
            for (uint16_t dy = 0; dy < size; ++dy) {
                pixel(x + dx, y + dy, color);
            }
        }
    }

    /**
     * @brief Draw a line using Bresenham's algorithm
     * @param x0 Start X-coordinate
     * @param y0 Start Y-coordinate
     * @param x1 End X-coordinate
     * @param y1 End Y-coordinate
     * @param color 16-bit color in RGB565 format
     * @param size Thickness of the line in pixels (default 1)
     */
    void line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
              uint16_t color, uint16_t size = 1) noexcept {
        int dx = abs(int(x1) - int(x0));
        int sx = (x0 < x1) ? 1 : -1;
        int dy = -abs(int(y1) - int(y0));
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx + dy;
        while (true) {
            point(x0, y0, color, size);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 >= dy) { err += dy; x0 += sx; }
            if (e2 <= dx) { err += dx; y0 += sy; }
        }
    }

    /**
     * @brief Draw a horizontal line
     * @param x Starting X-coordinate
     * @param y Y-coordinate
     * @param len Length in pixels
     * @param color 16-bit color in RGB565 format
     * @param size Thickness in pixels (default 1)
     */
    inline void hline(uint16_t x, uint16_t y, uint16_t len,
                      uint16_t color, uint16_t size = 1) noexcept {
        for (uint16_t i = 0; i < len; ++i) {
            point(x + i, y, color, size);
        }
    }

    /**
     * @brief Draw a vertical line
     * @param x X-coordinate
     * @param y Starting Y-coordinate
     * @param len Length in pixels
     * @param color 16-bit color in RGB565 format
     * @param size Thickness in pixels (default 1)
     */
    inline void vline(uint16_t x, uint16_t y, uint16_t len,
                      uint16_t color, uint16_t size = 1) noexcept {
        for (uint16_t i = 0; i < len; ++i) {
            point(x, y + i, color, size);
        }
    }

    /**
     * @brief Draw a rectangle (filled or outline)
     * @param x0 Top-left X-coordinate
     * @param y0 Top-left Y-coordinate
     * @param x1 Bottom-right X-coordinate
     * @param y1 Bottom-right Y-coordinate
     * @param color 16-bit color in RGB565 format
     * @param size Border thickness in pixels (default 1)
     * @param fill True for filled rectangle, false for outline (default false)
     */
    void rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
              uint16_t color, uint16_t size = 1, bool fill = false) noexcept {
        if (fill) {
            for (uint16_t y = y0; y <= y1; ++y) {
                hline(x0, y, x1 - x0 + 1, color, size);
            }
        } else {
            hline(x0, y0, x1 - x0 + 1, color, size);
            hline(x0, y1, x1 - x0 + 1, color, size);
            vline(x0, y0, y1 - y0 + 1, color, size);
            vline(x1, y0, y1 - y0 + 1, color, size);
        }
    }

    /**
     * @brief Draw a circle (midpoint algorithm)
     * @param cx Center X-coordinate
     * @param cy Center Y-coordinate
     * @param r Radius in pixels
     * @param color 16-bit color in RGB565 format
     * @param size Thickness of the circle line or fill thickness (default 1)
     * @param fill True for filled circle, false for outline (default false)
     */
    void circle(uint16_t cx, uint16_t cy, uint16_t r,
                uint16_t color, uint16_t size = 1, bool fill = false) noexcept {
        int x = 0, y = r;
        int d = 3 - 2 * int(r);
        auto plot = [&](int dx, int dy) {
            if (fill) {
                hline(cx - dx, cy + dy, 2 * dx + 1, color, size);
                hline(cx - dx, cy - dy, 2 * dx + 1, color, size);
                hline(cx - dy, cy + dx, 2 * dy + 1, color, size);
                hline(cx - dy, cy - dx, 2 * dy + 1, color, size);
            } else {
                point(cx + dx, cy + dy, color, size);
                point(cx - dx, cy + dy, color, size);
                point(cx + dx, cy - dy, color, size);
                point(cx - dx, cy - dy, color, size);
                point(cx + dy, cy + dx, color, size);
                point(cx - dy, cy + dx, color, size);
                point(cx + dy, cy - dx, color, size);
                point(cx - dy, cy - dx, color, size);
            }
        };
        while (y >= x) {
            plot(x, y);
            ++x;
            if (d > 0) {
                --y;
                d += 4 * (x - y) + 10;
            } else {
                d += 4 * x + 6;
            }
        }
    }

    /**
     * @brief Draw text string using provided Font class
     * @param x Starting X-coordinate
     * @param y Starting Y-coordinate
     * @param str Null-terminated string to draw
     * @param font Reference to Font object
     * @param fg Foreground color in RGB565 format
     * @param bg Background color in RGB565 format
     */
    inline void text(uint16_t x, uint16_t y,
                     const char* str, const Font& font,
                     uint16_t fg, uint16_t bg) noexcept {
        uint16_t cx = x, cy = y;
        while (*str) {
            this->ch(cx, cy, *str++, const_cast<Font*>(&font), bg, fg);
            cx += font.width;
            if (cx + font.width > width) {
                cx = x;
                cy += font.height;
            }
            if (cy + font.height > height) break;
        }
    }

    /**
     * @brief Upload buffer to display via SPI (blocking)
     * @param spi SPI instance (default spi0)
     * @param cs Chip select pin
     * @param dc Data/command pin
     */
    void display(spi_inst_t* spi = spi0, int cs = 0, int dc = 0) noexcept {
        gpio_put(cs, 0);
        gpio_put(dc, 1);
        spi_write16_blocking(spi, buffer, width * height);
        gpio_put(cs, 1);
    }
};

#endif //FRAMEBUFFER_H
