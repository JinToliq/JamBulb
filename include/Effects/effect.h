#pragma once

#include <cstdint>
#include "matrix_map.h"
#include <FastLED.h>

class Effect {
public:
    virtual ~Effect() = default;

    // Update animation state. dt is seconds elapsed * speed (see main loop)
    virtual void Update(float dt) = 0;

    // Render should write into the leds array using map.index(x,y)
    virtual void Render(CRGB *leds, const MatrixMap &map) = 0;

    // Called when effect becomes active
    virtual void Reset() {}
};
