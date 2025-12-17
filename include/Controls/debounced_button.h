#pragma once

#include <Arduino.h>

class DebouncedButton {
public:
    DebouncedButton(int pin, unsigned long debounceMs = 50);

    // Call regularly; returns true if stable state changed
    bool update();
    bool isPressed() const;

private:
    int pin;
    unsigned long debounceMs;
    bool state;
    bool lastStable;
    bool lastRaw;
    unsigned long lastChange;
};
