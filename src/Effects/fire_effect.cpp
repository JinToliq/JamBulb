#include "Effects/fire_effect.h"

FireEffect::FireEffect(uint8_t width, uint8_t height)
    : m_width(width), m_height(height), m_time(0.0f), m_yOffset(0.0f) {}

void FireEffect::Reset() {
    m_time = 0.0f;
    m_yOffset = 0.0f;
}

void FireEffect::Update(float dt) {
    m_time += dt * FIRE_TIME_SCALE;
    m_yOffset += dt * FIRE_SPEED;
}

void FireEffect::Render(CRGB *leds, const MatrixMap &map) {
    // Use 16-bit time for FastLED noise functions
    uint16_t timeVal = static_cast<uint16_t>(m_time * 65535.0f);
    
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            // Calculate noise coordinates with upward offset
            uint16_t xCoord = static_cast<uint16_t>(x * FIRE_NOISE_SCALE * 65535.0f / m_width);
            uint16_t yCoord = static_cast<uint16_t>((y + m_yOffset) * FIRE_NOISE_SCALE * 65535.0f / m_height);
            
            // Get 3D Perlin noise value
            uint8_t noiseVal = inoise8(xCoord, yCoord, timeVal);
            
            // Fire dissolves as it rises - reduce brightness at the top
            float heightFactor = 1.0f - (static_cast<float>(y) / m_height);
            heightFactor = heightFactor * heightFactor; // Square for more dramatic falloff
            
            // Combine noise with height-based dissolve
            uint8_t brightness = static_cast<uint8_t>(noiseVal * heightFactor);
            
            // Map noise to hue (red to yellow)
            uint8_t hue = FIRE_BASE_HUE + static_cast<uint8_t>((noiseVal * FIRE_HUE_RANGE) / 255);
            
            // Map noise to saturation (more yellow when hotter)
            uint8_t saturation = FIRE_MIN_SAT + static_cast<uint8_t>((noiseVal * (FIRE_MAX_SAT - FIRE_MIN_SAT)) / 255);
            
            uint16_t i = map.index(x, y);
            leds[i] = CHSV(hue, saturation, brightness);
        }
    }
}
