#pragma once

#include "Effects/effect.h"

// Lava effect using 3D Perlin noise with slower, rolling movement
class LavaEffect : public Effect {
public:
    LavaEffect(uint8_t width, uint8_t height);
    void Update(float dt) override;
    void Render(CRGB *leds, const MatrixMap &map) override;
    void Reset() override;

private:
    uint8_t m_width;
    uint8_t m_height;
    float m_time;      // Time accumulator for Z dimension of noise
    float m_xOffset;   // X offset for horizontal movement
    float m_yOffset;   // Y offset for vertical movement
    
    // Color configuration constants
    static constexpr uint8_t LAVA_BASE_HUE = 0;        // Red
    static constexpr uint8_t LAVA_HUE_RANGE = 20;     // Red to deep orange
    static constexpr uint8_t LAVA_MIN_SAT = 230;      // Very high saturation
    static constexpr uint8_t LAVA_MAX_SAT = 255;      // Full saturation
    static constexpr uint8_t LAVA_MIN_BRIGHT = 5;    // Dimmer areas (cooling lava)
    static constexpr uint8_t LAVA_MAX_BRIGHT = 255;   // Bright areas (hot lava)
    static constexpr float LAVA_X_SPEED = 0.02f;      // Horizontal movement speed
    static constexpr float LAVA_Y_SPEED = 0.03f;      // Vertical movement speed
    static constexpr float LAVA_NOISE_SCALE = 0.15f;  // Noise scale (larger = chunkier)
    static constexpr float LAVA_TIME_SCALE = 0.03f;   // Time scale for Z dimension
};
