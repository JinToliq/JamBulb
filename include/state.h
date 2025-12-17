#pragma once

#include <cstdint>
#include "Config/runtime_config.h"

// RuntimeState holds runtime settings and persists them to Preferences.
class State {
public:
    State() = default;

    int currentEffectIndex = 0;
    float speed = SPEED_DEFAULT;
    uint8_t brightness = BRIGHTNESS_DEFAULT;

    // Load persisted settings; provides effectCount to clamp the active effect
    void load(int effectCount);

    // Persist current settings
    void save() const;
};
