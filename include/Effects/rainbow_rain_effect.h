#pragma once

#include "Effects/effect.h"

class RainbowRainEffect : public Effect {
public:
    RainbowRainEffect(uint8_t width, uint8_t height);
    void Update(float dt) override;
    void Render(CRGB *leds, const MatrixMap &map) override;
    void Reset() override;

private:
    uint8_t m_width;
    uint8_t m_height;
    float m_phase; // changes over time
};
