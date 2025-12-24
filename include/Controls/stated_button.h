#pragma once

#include <Arduino.h>

class StatedButton {
public:
    StatedButton(int pin, unsigned long debounceMs, unsigned long clickThresholdMs);

    void update();
    bool isPressed() const { return _isPressed; }
    bool isClicked() const { return _isClicked; }
    bool changedThisFrame() const { return _changedThisFrame; }

private:
    const int _pin;
    const unsigned long _debounceMs;
    const unsigned long _clickThresholdMs;
    unsigned long _lastChangeMs;
    unsigned long _lastStableChangeMs;

    bool _changedThisFrame;
    bool _isPressed;
    bool _isClicked;
    bool _lastRaw;

    bool readRaw() const;
};
