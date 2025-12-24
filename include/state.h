#pragma once

#include <cstdint>
#include <Arduino.h>
#include "Config/runtime_config.h"

// RuntimeState holds runtime settings and persists them to Preferences.
class State {
public:
    State() = default;

    unsigned int currentEffectIndex = 0;
    float speed = SPEED_DEFAULT;
    uint8_t brightness = BRIGHTNESS_DEFAULT;
    String wifiPassword = WIFI_DEFAULT_PASSWORD;

    void load();
    void save() const;
};
