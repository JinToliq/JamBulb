#include "Effects/lava_effect.h"

LavaEffect::LavaEffect(uint8_t width, uint8_t height)
    : m_width(width), m_height(height), m_time(0.0f), m_xOffset(0.0f), m_yOffset(0.0f) {}

void LavaEffect::Reset() {
    m_time = 0.0f;
    m_xOffset = 0.0f;
    m_yOffset = 0.0f;
}

void LavaEffect::Update(float dt) {
    m_time += dt * LAVA_TIME_SCALE;
    m_xOffset += dt * LAVA_X_SPEED;
    m_yOffset += dt * LAVA_Y_SPEED;
}

void LavaEffect::Render(CRGB *leds, const MatrixMap &map) {
    // Use 16-bit time for FastLED noise functions
    uint16_t timeVal = static_cast<uint16_t>(m_time * 65535.0f);
    
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            // Calculate noise coordinates with moving offset (creates flowing effect)
            uint16_t xCoord = static_cast<uint16_t>((x + m_xOffset) * LAVA_NOISE_SCALE * 65535.0f / m_width);
            uint16_t yCoord = static_cast<uint16_t>((y + m_yOffset) * LAVA_NOISE_SCALE * 65535.0f / m_height);
            
            // Get 3D Perlin noise value
            uint8_t noiseVal = inoise8(xCoord, yCoord, timeVal);
            
            // Use a second layer of noise for more complex patterns
            uint16_t xCoord2 = static_cast<uint16_t>((x - m_xOffset * 0.5f) * LAVA_NOISE_SCALE * 2.0f * 65535.0f / m_width);
            uint16_t yCoord2 = static_cast<uint16_t>((y - m_yOffset * 0.5f) * LAVA_NOISE_SCALE * 2.0f * 65535.0f / m_height);
            uint8_t noiseVal2 = inoise8(xCoord2, yCoord2, timeVal + 30000);
            
            // Combine noise layers
            uint8_t combinedNoise = (noiseVal + noiseVal2) / 2;
            
            // Map noise to hue (red to orange for lava)
            uint8_t hue = LAVA_BASE_HUE + static_cast<uint8_t>((combinedNoise * LAVA_HUE_RANGE) / 255);
            
            // Map noise to saturation
            uint8_t saturation = LAVA_MIN_SAT + static_cast<uint8_t>((combinedNoise * (LAVA_MAX_SAT - LAVA_MIN_SAT)) / 255);
            
            // Map noise to brightness (lava has bright and dark areas)
            uint8_t brightness = LAVA_MIN_BRIGHT + static_cast<uint8_t>((combinedNoise * (LAVA_MAX_BRIGHT - LAVA_MIN_BRIGHT)) / 255);
            
            uint16_t i = map.index(x, y);
            leds[i] = CHSV(hue, saturation, brightness);
        }
    }
}
