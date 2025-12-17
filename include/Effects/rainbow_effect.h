#pragma once

#include "Effects/effect.h"

// Simple Rainbow effect: entire matrix uses the same hue, which cycles over time
class RainbowEffect : public Effect {
public:
    RainbowEffect(uint8_t width, uint8_t height);
    void Update(float dt) override;
    void Render(CRGB *leds, const MatrixMap &map) override;
    void Reset() override;

private:
    uint8_t m_width;
    uint8_t m_height;
    float m_phase; // 0..1 phase used to compute hue
};
