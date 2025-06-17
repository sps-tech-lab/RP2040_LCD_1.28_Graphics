#########################################################################################
#     __________  _   ________   __________  _   ___    ____________  ________________  #
#    / ____/ __ \/ | / /_  __/  / ____/ __ \/ | / / |  / / ____/ __ \/_  __/ ____/ __ \ #
#   / /_  / / / /  |/ / / /    / /   / / / /  |/ /| | / / __/ / /_/ / / / / __/ / /_/ / #
#  / __/ / /_/ / /|  / / /    / /___/ /_/ / /|  / | |/ / /___/ _, _/ / / / /___/ _, _/  #
# /_/    \____/_/ |_/ /_/     \____/\____/_/ |_/  |___/_____/_/ |_| /_/ /_____/_/ |_|   #
#########################################################################################
#                                     SPS :: 2025                                       #
#########################################################################################
"""
Convert any ttf font to fixed-size font.cpp file suitable for current framebuffer version

Usage:
  python3 font_converter.py <your_font.ttf> -s <size>

Arguments:

  -h, --help
        show this help message and exit

  -s, --size SIZE
        Font pixel height (default: 12)

  -w, --width WIDTH
        Font pixel width (default: 10)
        [Hint] check conversion output to see if any glyphs will be cropped

  -o, --output OUTPUT
        Output .cpp file (default: font.cpp)

Examples:
  python3 ./scripts/font_converter.py ./Roboto-Black.ttf -s 24 -o font24.cpp
"""

import freetype
import sys
import os
import argparse
import math

CHAR_RANGE = range(32, 127)  # Printable ASCII

def render_glyph_bitmap(face, char, width, height):
    face.load_char(char, freetype.FT_LOAD_RENDER)
    bitmap = face.glyph.bitmap

    # Create empty fixed-size buffer
    fixed = [[0] * width for _ in range(height)]

    # Center smaller bitmaps into fixed box
    x_off = max((width - bitmap.width) // 2, 0)
    y_off = max((height - bitmap.rows), 0)  # Top-aligned, can change to center if preferred

    for y in range(min(bitmap.rows, height)):
        for x in range(min(bitmap.width, width)):
            pixel = bitmap.buffer[y * bitmap.width + x]
            if pixel > 128:
                fixed[y + y_off][x + x_off] = 1

    return fixed

def pack_bitmap_rows(bitmap, width):
    """Pack each row of bits into bytes."""
    bytes_per_row = math.ceil(width / 8)
    packed = []

    for row in bitmap:
        byte = 0
        bits_filled = 0
        for bit in row:
            byte = (byte << 1) | bit
            bits_filled += 1
            if bits_filled == 8:
                packed.append(byte)
                byte = 0
                bits_filled = 0

        if bits_filled > 0:
            byte <<= (8 - bits_filled)
            packed.append(byte)

    return packed

def main():
    parser = argparse.ArgumentParser(description="Convert TTF to fixed-size C++ bitmap font array")
    parser.add_argument("font_path", help="Path to the .ttf font file")
    parser.add_argument("-s", "--size", type=int, default=12, help="Font pixel height (default: 12)")
    parser.add_argument("-w", "--width", type=int, default=10, help="Font pixel width (default: 10)")
    parser.add_argument("-o", "--output", default="font.cpp", help="Output .cpp file (default: font.cpp)")
    args = parser.parse_args()

    if not os.path.exists(args.font_path):
        print(f"❌ Font file not found: {args.font_path}")
        sys.exit(1)

    try:
        face = freetype.Face(args.font_path)
        face.set_pixel_sizes(0, args.size)

        # Measure maximum glyph width in the font
        max_width_detected = 0
        for char_code in CHAR_RANGE:
            char = chr(char_code)
            face.load_char(char, freetype.FT_LOAD_RENDER)
            bitmap = face.glyph.bitmap
            if bitmap.width > max_width_detected:
                max_width_detected = bitmap.width

        # Warn if user's width is smaller than any glyph
        print(f"ℹ️  Detected max glyph width: {max_width_detected} pixels")
        print(f"ℹ️  Using specified output width: {args.width} pixels")
        if args.width < max_width_detected:
            print("⚠️  Warning: Some glyphs will be cropped horizontally!")

    except freetype.ft_errors.FT_Exception as e:
        print(f"❌ Failed to load font: {e}")
        sys.exit(2)

    bytes_per_row = math.ceil(args.width / 8)
    bytes_per_glyph = bytes_per_row * args.size

    with open(args.output, "w") as out:
        out.write('#include "fonts.hpp"\n\n')
        out.write(f"const uint8_t Font{args.size}_Table [] = {{\n")

        for char_code in CHAR_RANGE:
            char = chr(char_code)
            fixed_bitmap = render_glyph_bitmap(face, char, args.width, args.size)
            packed_bytes = pack_bitmap_rows(fixed_bitmap, args.width)

            out.write(f"    // @{char_code * bytes_per_glyph} '{char}' ({args.width}x{args.size} pixels)\n")
            for i in range(0, len(packed_bytes), 2):
                b1 = f"0x{packed_bytes[i]:02X}"
                b2 = f", 0x{packed_bytes[i+1]:02X}" if i+1 < len(packed_bytes) else ""
                out.write(f"    {b1}{b2},\n")
            out.write("\n")

        out.write("};\n\n")
        out.write(f"font font{args.size}(Font{args.size}_Table, {args.width}, {args.size});\n")

    print(f"✅ Font generated: {args.output} ({len(CHAR_RANGE)} glyphs × {bytes_per_glyph} bytes)")

if __name__ == "__main__":
    main()
