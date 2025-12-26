#pragma once

#include "Effects/effect.h"

// Fire effect using 3D Perlin noise with upward movement and dissolve
class FireEffect : public Effect {
public:
    FireEffect(uint8_t width, uint8_t height);
    void Update(float dt) override;
    void Render(CRGB *leds, const MatrixMap &map) override;
    void Reset() override;

private:
    uint8_t m_width;
    uint8_t m_height;
    float m_time;      // Time accumulator for Z dimension of noise
    float m_yOffset;   // Y offset for upward movement
    
    // Color configuration constants
    static constexpr uint8_t FIRE_BASE_HUE = 0;        // Red
    static constexpr uint8_t FIRE_HUE_RANGE = 30;     // Red to orange-yellow
    static constexpr uint8_t FIRE_MIN_SAT = 200;      // High saturation
    static constexpr uint8_t FIRE_MAX_SAT = 255;      // Full saturation
    static constexpr float FIRE_SPEED = 0.15f;        // Upward movement speed
    static constexpr float FIRE_NOISE_SCALE = 0.08f;  // Noise scale factor
    static constexpr float FIRE_TIME_SCALE = 0.05f;   // Time scale for Z dimension
};
