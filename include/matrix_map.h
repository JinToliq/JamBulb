#pragma once

#include <cstdint>

// MatrixMap maps 2D coordinates (x,y) with origin at bottom-left to a 1D index
// matching the physical wiring. By default it uses a "snake" (zig-zag)
// wiring where rows alternate direction.

class MatrixMap {
public:
    MatrixMap(uint8_t width = 16, uint8_t height = 16);

    // Rotation support removed; mapping uses fixed orientation (origin bottom-left)

    // Return 1D index for logical coordinates x,y (x from 0..width-1, y from 0..height-1)
    // coordinates assume origin is bottom-left
    uint16_t index(int x, int y) const;

    uint8_t width() const { return m_width; }
    uint8_t height() const { return m_height; }

private:
    uint8_t m_width;
    uint8_t m_height;
};
