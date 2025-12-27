#pragma once

#include "Effects/effect.h"

class FireEffect : public Effect {
public:
    FireEffect(uint8_t width, uint8_t height);
    void Update(float dt) override;
    void Render(CRGB *leds, const MatrixMap &map) override;
    void Reset() override;

private:
    uint8_t m_width;
    uint8_t m_height;
    float m_time;
    
    static constexpr float TIME_MULTIPLIER = 0.05f;
    static constexpr uint8_t FIRE_BASE_HUE = 0;
    static constexpr uint8_t FIRE_HUE_RANGE = 30;
    static constexpr uint8_t FIRE_MIN_SAT = 200;
    static constexpr uint8_t FIRE_MAX_SAT = 255;
    static constexpr float FIRE_SPEED = 0.15f;
    static constexpr float FIRE_NOISE_SCALE = 0.09f;
};
