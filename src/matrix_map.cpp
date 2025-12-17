#include "matrix_map.h"

MatrixMap::MatrixMap(uint8_t width, uint8_t height)
    : m_width(width), m_height(height) {
}

uint16_t MatrixMap::index(int x, int y) const {
    // Fixed orientation mapping (origin bottom-left)
    int tx = x;
    int ty = y;

    // Now tx,ty are in standard orientation with origin bottom-left
    // Snake wiring: even rows (0-based from bottom) left->right, odd rows right->left
    int row = ty;
    int idx;
    if ((row & 1) == 0) {
        // left to right
        idx = row * m_width + tx;
    } else {
        // right to left
        idx = row * m_width + (m_width - 1 - tx);
    }
    return static_cast<uint16_t>(idx);
}
