#include "Effects/rainbow_rain_effect.h"

RainbowRainEffect::RainbowRainEffect(uint8_t width, uint8_t height)
    : m_width(width), m_height(height), m_phase(0.0f) {}

void RainbowRainEffect::Reset() {
    m_phase = 1.0f;
}

void RainbowRainEffect::Update(float dt) {
    // advance phase: 0..1 per second
    m_phase += dt * 0.1f; // scale down to make it pleasant; speed controlled externally
    if (m_phase > 1.0f)
        m_phase -= static_cast<int>(m_phase);
}

void RainbowRainEffect::Render(CRGB *leds, const MatrixMap &map) {
    // For each pixel compute hue based on Y position (top to bottom) plus phase
    for (int y = 0; y < m_height; ++y) {
        // top (m_height-1) should have a different hue than bottom (0)
        uint8_t rowHue = static_cast<uint8_t>((255 * (m_height - 1 - y)) / (m_height - 1));
        uint8_t hueShift = static_cast<uint8_t>(m_phase * 255 * 6);
        uint8_t hue = rowHue + hueShift;
        for (int x = 0; x < m_width; ++x) {
            uint16_t i = map.index(x, y);
            leds[i] = CHSV(hue, 255, 255);
        }
    }
}
