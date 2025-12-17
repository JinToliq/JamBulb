#include "Effects/rainbow_effect.h"

RainbowEffect::RainbowEffect(uint8_t width, uint8_t height)
    : m_width(width), m_height(height), m_phase(0.0f) {}

void RainbowEffect::Reset() {
    m_phase = 1.0f;
}

void RainbowEffect::Update(float dt) {
    // advance phase: 0..1 (wrap)
    m_phase += dt * 0.05f; // slow, pleasant rotation
    if (m_phase > 1.0f)
        m_phase -= static_cast<int>(m_phase);
}

void RainbowEffect::Render(CRGB *leds, const MatrixMap &map) {
    // Uniform hue for the entire matrix
    uint8_t hue = static_cast<uint8_t>(m_phase * 255);
    // fill entire buffer
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            uint16_t i = map.index(x, y);
            leds[i] = CHSV(hue, 255, 255);
        }
    }
}
