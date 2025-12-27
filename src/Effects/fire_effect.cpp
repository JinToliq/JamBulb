#include "Effects/fire_effect.h"

FireEffect::FireEffect(uint8_t width, uint8_t height)
    : m_width(width), m_height(height), m_time(0.0f) {}

void FireEffect::Reset() {
    m_time = 0.0f;
}

void FireEffect::Update(float dt) {
    m_time += dt * TIME_MULTIPLIER;
}

void FireEffect::Render(CRGB *leds, const MatrixMap &map) {

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            auto xCoord = static_cast<uint16_t>(x * FIRE_NOISE_SCALE);
            auto yCoord = static_cast<uint16_t>((y + m_time) * FIRE_NOISE_SCALE);
            
            auto noiseValue = inoise8(xCoord, yCoord, 0);
            auto heightFactor = 1.0f - (static_cast<float>(y) / m_height);
            
            auto brightness = static_cast<uint8_t>(noiseValue * heightFactor);
            auto hue = FIRE_BASE_HUE + static_cast<uint8_t>((noiseValue * FIRE_HUE_RANGE) / 255);
            auto saturation = FIRE_MIN_SAT + static_cast<uint8_t>((noiseValue * (FIRE_MAX_SAT - FIRE_MIN_SAT)) / 255);
            leds[map.index(x, y)] = CHSV(hue, saturation, brightness);
        }
    }
}
